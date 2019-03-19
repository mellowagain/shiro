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

#include "../database/tables/user_table.hh"
#include "../pp/pp_recalculator.hh"
#include "../users/user_activity.hh"
#include "../users/user_manager.hh"
#include "../users/user_punishments.hh"
#include "../utils/play_mode.hh"
#include "ranking_helper.hh"

int32_t shiro::ranking::helper::get_leaderboard_position(uint8_t mode, std::string username) {
    if (username.empty())
        return 0;

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};

    auto result = db(select(all_of(user_table)).from(user_table).unconditionally());

    if (result.empty())
        return 0;

    std::vector<std::pair<std::string, float>> users;

    for (const auto &row : result) {
        if ((int32_t) row.id == 1)
            continue;

        if (!users::punishments::has_scores(row.id))
            continue;

        switch (mode) {
            case (uint8_t) utils::play_mode::standard:
                users.emplace_back(std::make_pair(row.username, row.pp_std));
                break;
            case (uint8_t) utils::play_mode::taiko:
                users.emplace_back(std::make_pair(row.username, row.pp_taiko));
                break;
            case (uint8_t) utils::play_mode::fruits:
                users.emplace_back(std::make_pair(row.username, row.pp_ctb));
                break;
            case (uint8_t) utils::play_mode::mania:
                users.emplace_back(std::make_pair(row.username, row.pp_mania));
                break;
        }
    }

    std::sort(users.begin(), users.end(), [](const std::pair<std::string, float> &s_left, const std::pair<std::string, float> &s_right) {
        return s_left.second > s_right.second;
    });

    for (size_t i = 0; i < users.size(); i++) {
        const auto &[name, _] = users.at(i);

        if (username == name)
            return i + 1;
    }

    return 0;
}

std::string shiro::ranking::helper::get_leaderboard_user(uint8_t mode, int32_t pos) {
    if (pos < 1)
        return "";

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};

    auto result = db(select(all_of(user_table)).from(user_table).unconditionally());

    if (result.empty())
        return "";

    std::vector<std::pair<std::string, float>> users;

    for (const auto &row : result) {
        if ((int32_t) row.id == 1)
            continue;

        if (!users::punishments::has_scores(row.id))
            continue;

        switch (mode) {
            case (uint8_t) utils::play_mode::standard:
                users.emplace_back(std::make_pair(row.username, row.pp_std));
                break;
            case (uint8_t) utils::play_mode::taiko:
                users.emplace_back(std::make_pair(row.username, row.pp_taiko));
                break;
            case (uint8_t) utils::play_mode::fruits:
                users.emplace_back(std::make_pair(row.username, row.pp_ctb));
                break;
            case (uint8_t) utils::play_mode::mania:
                users.emplace_back(std::make_pair(row.username, row.pp_mania));
                break;
        }
    }

    std::sort(users.begin(), users.end(), [](const std::pair<std::string, float> &s_left, const std::pair<std::string, float> &s_right) {
        return s_left.second > s_right.second;
    });

    try {
        return users.at(pos - 1).first;
    } catch (const std::out_of_range &ex) {
        return "";
    }
}

int16_t shiro::ranking::helper::get_pp_for_user(uint8_t mode, std::string username) {
    if (username.empty())
        return 0;

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};

    auto result = db(select(all_of(user_table)).from(user_table).where(user_table.username == username).limit(1u));

    if (result.empty())
        return 0;

    const auto &row = result.front();

    switch (mode) {
        case (uint8_t) utils::play_mode::standard:
            return row.pp_std;
        case (uint8_t) utils::play_mode::taiko:
            return row.pp_taiko;
        case (uint8_t) utils::play_mode::fruits:
            return row.pp_ctb;
        case (uint8_t) utils::play_mode::mania:
            return row.pp_mania;
    }

    return 0;
}

void shiro::ranking::helper::recalculate_ranks(const shiro::utils::play_mode &mode) {
    // Global pp recalculation is currently in progress.
    if (pp::recalculator::in_progess())
        return;

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};

    auto result = db(select(all_of(user_table)).from(user_table).unconditionally());

    if (result.empty())
        return;

    std::vector<std::pair<int32_t, float>> users;

    for (const auto &row : result) {
        if ((int32_t) row.id == 1)
            continue;

        // This needs to be configurable for future updates
        // Users should be purged once there is a update to the performance calculator
        // They shouldn't be purged *every* time a new score is submitted
        //if (users::activity::is_inactive(row.id, mode))
        //    continue;

        switch (mode) {
            case utils::play_mode::standard:
                if ((int32_t) row.play_count_std <= 0)
                    continue;

                users.emplace_back(std::make_pair<int32_t, float>(row.id, row.pp_std));
                break;
            case utils::play_mode::taiko:
                if ((int32_t) row.play_count_taiko <= 0)
                    continue;

                users.emplace_back(std::make_pair<int32_t, float>(row.id, row.pp_taiko));
                break;
            case utils::play_mode::fruits:
                if ((int32_t) row.play_count_ctb <= 0)
                    continue;

                users.emplace_back(std::make_pair<int32_t, float>(row.id, row.pp_ctb));
                break;
            case utils::play_mode::mania:
                if ((int32_t) row.play_count_mania <= 0)
                    continue;

                users.emplace_back(std::make_pair<int32_t, float>(row.id, row.pp_mania));
                break;
        }
    }

    std::sort(users.begin(), users.end(), [](const std::pair<int32_t, float> &s_left, const std::pair<int32_t, float> &s_right) {
        return s_left.second > s_right.second;
    });

    // At this point, the users array is sorted by rank, this means the 0th element is rank #1

    for (size_t i = 0; i < users.size(); i++) {
        auto [user_id, pp] = users.at(i);
        int32_t rank = (int32_t) i + 1;

        std::shared_ptr<users::user> user = users::manager::get_user_by_id(user_id);

        switch (mode) {
            case utils::play_mode::standard:
                db(update(user_table).set(
                        user_table.rank_std = rank
                ).where(user_table.id == user_id));

                if (user != nullptr && user->stats.play_mode == (uint8_t) utils::play_mode::standard) {
                    user->stats.rank = rank;
                    user->presence.rank = rank;
                }
                break;
            case utils::play_mode::taiko:
                db(update(user_table).set(
                        user_table.rank_taiko = rank
                ).where(user_table.id == user_id));

                if (user != nullptr && user->stats.play_mode == (uint8_t) utils::play_mode::taiko) {
                    user->stats.rank = rank;
                    user->presence.rank = rank;
                }
                break;
            case utils::play_mode::fruits:
                db(update(user_table).set(
                        user_table.rank_ctb = rank
                ).where(user_table.id == user_id));

                if (user != nullptr && user->stats.play_mode == (uint8_t) utils::play_mode::fruits) {
                    user->stats.rank = rank;
                    user->presence.rank = rank;
                }
                break;
            case utils::play_mode::mania:
                db(update(user_table).set(
                        user_table.rank_mania = rank
                ).where(user_table.id == user_id));

                if (user != nullptr && user->stats.play_mode == (uint8_t) utils::play_mode::mania) {
                    user->stats.rank = rank;
                    user->presence.rank = rank;
                }
                break;
        }
    }

    io::osu_writer writer;

    // First we add all user stats/presence updates to the global writer
    users::manager::iterate([&writer](std::shared_ptr<users::user> online_user) {
        if (online_user->hidden)
            return;

        writer.user_stats(online_user->stats);
        writer.user_presence(online_user->presence);
    }, true);

    // After we have all user updates in the writer, we can send them out globally
    users::manager::iterate([&writer](std::shared_ptr<users::user> online_user) {
        online_user->queue.enqueue(writer);
    }, true);
}
