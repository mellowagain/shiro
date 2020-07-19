/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2020 Marc3842h, czapek
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

#ifndef SHIRO_MATCH_TEAM_TYPE_HH
#define SHIRO_MATCH_TEAM_TYPE_HH

#include <cstdint>

namespace shiro::utils {

    enum class match_team_type : uint8_t {
        head_to_head = 0,
        tag_coop = 1,
        team_vs = 2,
        tag_team_vs = 3

    };

    inline bool is_team(uint8_t type) {
        return type == (uint8_t) match_team_type::team_vs || type == (uint8_t) match_team_type::tag_team_vs;
    }

}

#endif  // SHIRO_MATCH_TEAM_TYPE_HH
