/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018 Marc3842h, czapek
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

namespace shiro::ranking::helper {

    int32_t get_leaderboard_position(uint8_t mode, std::string username);

    std::string get_leaderboard_user(uint8_t mode, int32_t pos);

    int16_t get_pp_for_user(uint8_t mode, std::string username);

    void recalculate_ranks(uint8_t mode);

}

#endif //SHIRO_RANKING_HELPER_HH
