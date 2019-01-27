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

void shiro::ranking::helper::recalculate_ranks(uint8_t mode) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};

    auto result = db(select(all_of(user_table)).from(user_table).unconditionally());

    if (result.empty())
        return;

    for (const auto &row : result) {
        if ((int32_t) row.id == 1)
            continue;

        int32_t play_count = 0;
        int32_t old_rank = 0;

        switch ((utils::play_mode) mode) {
            case utils::play_mode::standard:
                play_count = row.play_count_std;
                old_rank = row.rank_std;
                break;
            case utils::play_mode::taiko:
                play_count = row.play_count_taiko;
                old_rank = row.rank_taiko;
                break;
            case utils::play_mode::fruits:
                play_count = row.play_count_ctb;
                old_rank = row.rank_ctb;
                break;
            case utils::play_mode::mania:
                play_count = row.play_count_mania;
                old_rank = row.rank_mania;
                break;
        }

        if (play_count == 0)
            continue;

        int32_t rank = get_leaderboard_position(mode, row.username);

        if (rank == old_rank)
            return;

        switch ((utils::play_mode) mode) {
            case utils::play_mode::standard:
                db(update(user_table).set(
                        user_table.rank_std = rank
                ).where(user_table.id == row.id));
                break;
            case utils::play_mode::taiko:
                db(update(user_table).set(
                        user_table.rank_taiko = rank
                ).where(user_table.id == row.id));
                break;
            case utils::play_mode::fruits:
                db(update(user_table).set(
                        user_table.rank_ctb = rank
                ).where(user_table.id == row.id));
                break;
            case utils::play_mode::mania:
                db(update(user_table).set(
                        user_table.rank_mania = rank
                ).where(user_table.id == row.id));
                break;
        }

        std::shared_ptr<users::user> user = users::manager::get_user_by_id(row.id);

        if (user == nullptr)
            continue;

        io::osu_writer writer;

        user->stats.rank = rank;
        user->presence.rank = rank;

        writer.user_stats(user->stats);
        writer.user_presence(user->presence);

        for (const std::shared_ptr<users::user> &online_user : users::manager::online_users) {
            if (online_user->user_id == 1 || online_user->hidden)
                continue;

            online_user->queue.enqueue(writer);
        }
    }
}
