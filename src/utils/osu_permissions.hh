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

#ifndef SHIRO_OSU_PERMISSIONS_HH
#define SHIRO_OSU_PERMISSIONS_HH

#include <cstdint>

namespace shiro::utils {

    enum class osu_permissions : uint8_t {
        none = 0 << 0, // Light yellow
        normal = 1 << 0, // Light yellow
        bat = 1 << 1, // Red
        supporter = 1 << 2, // Yellow
        friend_ = 1 << 3, // Light blue
        peppy = 1 << 4, // Blue
        tournament = 1 << 5 // This bit will never be sent by us in user presence
    };

}

#endif  // SHIRO_OSU_PERMISSIONS_HH
