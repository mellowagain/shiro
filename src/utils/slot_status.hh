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

#ifndef SHIRO_SLOT_STATUS_HH
#define SHIRO_SLOT_STATUS_HH

#include <cstdint>

namespace shiro::utils {

    enum class slot_status : uint8_t {
        open = 1 << 0,
        locked = 1 << 1,
        not_ready = 1 << 2,
        ready = 1 << 3,
        no_map = 1 << 4,
        playing = 1 << 5,
        complete = 1 << 6,
        quit = 1 << 7

    };

    constexpr uint8_t has_player_status = (uint8_t) slot_status::not_ready |
            (uint8_t) slot_status::ready |
            (uint8_t) slot_status::no_map |
            (uint8_t) slot_status::playing |
            (uint8_t) slot_status::complete;

    constexpr uint8_t has_playing_status = (uint8_t) slot_status::playing |
                                          (uint8_t) slot_status::complete;

}

#endif  // SHIRO_SLOT_STATUS_HH
