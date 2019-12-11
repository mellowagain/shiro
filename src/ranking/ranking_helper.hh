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

#ifndef SHIRO_RANKING_HELPER_HH
#define SHIRO_RANKING_HELPER_HH

#include <cstdint>
#include <memory>
#include <string>

#include "../users/user.hh"
#include "../users/user_manager.hh"
#include "../users/user_punishments.hh"
#include "../utils/play_mode.hh"

namespace shiro::ranking::helper {

    std::string get_leaderboard_user(uint8_t mode, int32_t pos);

    int16_t get_pp_for_user(uint8_t mode, std::string username);

    void recalculate_ranks(const utils::play_mode &mode);

    #if defined(SEPARATE_RX_LEADERBOARDS)
        std::string get_leaderboard_user_rx(uint8_t mode, int32_t pos);

        int16_t get_pp_for_user_rx(uint8_t mode, std::string username);

        void recalculate_ranks_rx(const utils::play_mode &mode);
    #endif

    #if defined(SEPARATE_AP_LEADERBOARDS)
        std::string get_leaderboard_user_ap(uint8_t mode, int32_t pos);

        int16_t get_pp_for_user_ap(uint8_t mode, std::string username);

        void recalculate_ranks_ap(const utils::play_mode &mode);
    #endif

    template <typename t>
    inline std::string get_user_at_leaderboard_pos(t data, uint8_t mode, int32_t pos) {
        if (pos < 1)
            return "";

        std::vector<std::pair<int32_t, float>> users;

        for (const auto &row : data) {
            if ((int32_t) row.id == 1)
                continue;

            if (!users::punishments::has_scores(row.id))
                continue;

            switch (mode) {
                default:
                case (uint8_t) utils::play_mode::standard:
                    users.emplace_back(std::make_pair(row.id, row.pp_std));
                    break;
                case (uint8_t) utils::play_mode::taiko:
                    users.emplace_back(std::make_pair(row.id, row.pp_taiko));
                    break;
                case (uint8_t) utils::play_mode::fruits:
                    users.emplace_back(std::make_pair(row.id, row.pp_ctb));
                    break;
                case (uint8_t) utils::play_mode::mania:
                    users.emplace_back(std::make_pair(row.id, row.pp_mania));
                    break;
            }
        }

        std::sort(users.begin(), users.end(), [](const std::pair<int32_t, float> &s_left, const std::pair<int32_t, float> &s_right) {
            return s_left.second > s_right.second;
        });

        try {
            return users::manager::get_username_by_id(users.at(pos - 1).first);
        } catch (const std::out_of_range &ex) {
            return "";
        }
    }

    template <typename t>
    inline void recalculate_ranks(t data, const utils::play_mode &mode) {

    }

}

#endif //SHIRO_RANKING_HELPER_HH
