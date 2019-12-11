/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2019 Marc3842h, czapek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "../database/tables/score_table.hh"
#include "../database/tables/user_table.hh"
#include "../beatmaps/beatmap_helper.hh"
#include "../ranking/ranking_helper.hh"
#include "../scores/score.hh"
#include "../scores/score_helper.hh"
#include "../thirdparty/loguru.hh"
#include "../users/user.hh"
#include "../users/user_manager.hh"
#include "../utils/vector_utils.hh"
#include "../shiro.hh"
#include "pp_recalculator.hh"
#include "pp_score_metric.hh"

bool shiro::pp::recalculator::running = false;
size_t shiro::pp::recalculator::running_threads = 0;
std::shared_timed_mutex shiro::pp::recalculator::mutex;

void shiro::pp::recalculator::begin(shiro::utils::play_mode mode, uint32_t threads) {
    if (running) {
        LOG_F(ERROR, "PP recalculation was called while already recalculating.");
        return;
    }

    if (threads >= 2)
        threads /= 2; // Only use half of the available threads for pp recalculation

    std::vector<int32_t> users;
    std::string game_mode = utils::play_mode_to_string(mode);

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};

    auto result = db(select(all_of(user_table)).from(user_table).unconditionally());

    for (const auto &row : result) {
        // Bots usually don't have scores, so let's skip them
        if ((uint32_t) row.roles == 0xDEADCAFE)
            continue;

        int32_t play_count = 0;

        /*switch (mode) {
            case utils::play_mode::standard:
                play_count = (int32_t) row.play_count_std;
                break;
            case utils::play_mode::taiko:
                play_count = (int32_t) row.play_count_taiko;
                break;
            case utils::play_mode::fruits:
                play_count = (int32_t) row.play_count_ctb;
                break;
            case utils::play_mode::mania:
                play_count = (int32_t) row.play_count_mania;
                break;
        }*/

        // No need to recalculate users that don't have any scores
        if (play_count <= 0)
            continue;

        users.emplace_back(row.id);
    }

    if (users.empty())
        return;

    std::unique_lock<std::shared_timed_mutex> lock(mutex);
    running = true;
    lock.unlock(); // Unlock to avoid deadlocks

    LOG_F(INFO, "Starting pp recalculation in %s for %lu users with %u threads.", game_mode.c_str(), users.size(), threads);

    bool evenly_distributable = users.size() % threads == 0;
    ptrdiff_t chunk_size = users.size() / threads;

    if (!evenly_distributable)
        chunk_size += 1;

    // If there are more (or equal the amount of) users than threads, every thread can calculate up to one user.
    if (threads >= users.size()) {
        chunk_size = 1;
        LOG_F(WARNING, "Thread amount is bigger or equal the amount of users.");
    }

    auto chunks = utils::chunk(users.begin(), users.end(), chunk_size);

    if (threads < users.size())
        LOG_F(INFO, "Distributing work across %u threads: every thread calculates up to %lu users. (%lu chunks)", threads, chunk_size, chunks.size());

    for (size_t i = 0; i < chunks.size(); i++) {
        auto [begin, end] = chunks.at(i);

        std::vector<int32_t> chunked_users(begin, end);
        std::thread thread(recalculate, mode, chunked_users);

        std::stringstream stream;
        stream << "0x" << thread.get_id();

        std::unique_lock<std::shared_timed_mutex> lock(mutex);
        running_threads++;
        lock.unlock(); // Unlock to avoid deadlocks

        LOG_F(INFO, "Thread %s started recalculating with chunk %lu. (%lu users)", stream.str().c_str(), i, chunked_users.size());

        // Let the thread do it's work
        thread.detach();
    }

    io::osu_writer writer;
    writer.announce(
            "Global PP recalculation has begun for all scores in " + game_mode + ". "
            "PP on scores may not match user overall pp amount. "
            "Global rank and user pp updates have been paused."
    );

    users::manager::iterate([&writer](std::shared_ptr<users::user> user) {
        user->queue.enqueue(writer);
    }, true);

    LOG_F(INFO, "All recalculation threads have been started. Let's get this train rolling.");
}

void shiro::pp::recalculator::end(shiro::utils::play_mode mode) {
    std::unique_lock<std::shared_timed_mutex> shared_lock(mutex);

    running_threads--;

    if (running_threads > 0) {
        LOG_F(INFO, "Still %lu threads running.", running_threads);
        return;
    }

    running = false;

    // Recalculate overall pp for all users now
    // TODO: This code is repeated in user_stats.cc, needs to be refactored asap
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};

    auto result = db(select(all_of(user_table)).from(user_table).unconditionally());

    for (const auto &row : result) {
        std::vector<scores::score> scores = scores::helper::fetch_top100_user(mode, row.id);
        float raw_pp = 0; // Here it is a float to keep decimal points, round it when setting final pp value

        for (size_t i = 0; i < scores.size(); i++) {
            scores::score score = scores.at(i);

            raw_pp += (score.pp * std::pow(0.95, i));
        }

        std::shared_ptr<users::user> user = users::manager::get_user_by_id(row.id);
        int16_t pp = std::clamp(static_cast<int16_t>(raw_pp), (int16_t) 0, std::numeric_limits<int16_t>::max());

        switch (mode) {
            case utils::play_mode::standard:
                //db(update(user_table).set(user_table.pp_std = pp).where(user_table.id == row.id));

                if (user != nullptr && user->stats.play_mode == (uint8_t) utils::play_mode::standard)
                    user->stats.pp = pp;

                break;
            case utils::play_mode::taiko:
                //db(update(user_table).set(user_table.pp_taiko = pp).where(user_table.id == row.id));

                if (user != nullptr && user->stats.play_mode == (uint8_t) utils::play_mode::taiko)
                    user->stats.pp = pp;

                break;
            case utils::play_mode::fruits:
                //db(update(user_table).set(user_table.pp_ctb = pp).where(user_table.id == row.id));

                if (user != nullptr && user->stats.play_mode == (uint8_t) utils::play_mode::fruits)
                    user->stats.pp = pp;

                break;
            case utils::play_mode::mania:
                //db(update(user_table).set(user_table.pp_mania = pp).where(user_table.id == row.id));

                if (user != nullptr && user->stats.play_mode == (uint8_t) utils::play_mode::mania)
                    user->stats.pp = pp;

                break;
        }
    }

    // Recalculate global ranks now that user pp is updated
    ranking::helper::recalculate_ranks(mode);

    io::osu_writer writer;
    writer.announce("PP recalculation has ended. Your pp amount and global rank have been updated.");

    users::manager::iterate([&writer](std::shared_ptr<users::user> user) {
        user->refresh_stats();
        user->queue.enqueue(writer);
    }, true);
}

bool shiro::pp::recalculator::in_progess() {
    std::shared_lock<std::shared_timed_mutex> lock(mutex);
    return running;
}

void shiro::pp::recalculator::recalculate(shiro::utils::play_mode mode, std::vector<int32_t> users) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    for (int32_t user_id : users) {
        std::vector<scores::score> scores = scores::helper::fetch_all_user_scores(user_id);

        if (scores.empty())
            continue;

        for (const scores::score &score : scores) {
            beatmaps::beatmap map;
            map.beatmap_md5 = score.beatmap_md5;

            // On recalculation we would hit osu! API too often so we exclusively allow database fetching
            if (!map.fetch_db()) {
                LOG_F(WARNING, "Score #%i was set on a beatmap that is not in the database. Skipping...", score.id);
                continue;
            }

            if (!beatmaps::helper::awards_pp(beatmaps::helper::fix_beatmap_status(map.ranked_status)))
                continue;

            float pp = pp::calculate(map, score);
            float abs_difference = std::fabs(pp - score.pp);

            bool increase = pp > score.pp;
            const char *prefix = increase ? "+" : "-";

            if (abs_difference > 1)
                LOG_F(MAX, "Recalculation: Score #%i (user id %i): %fpp -> %fpp (%s%fpp)", score.id, user_id, score.pp, pp, prefix, abs_difference);

            db(update(score_table).set(score_table.pp = pp).where(score_table.id == score.id));
        }
    }

    std::stringstream stream;
    stream << "0x" << std::this_thread::get_id();

    LOG_F(INFO, "Thread %s finished pp calculation.", stream.str().c_str());

    // We're done, let the end callback know that
    end(mode);
}
