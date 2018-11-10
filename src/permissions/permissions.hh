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

#ifndef SHIRO_PERMISSIONS_HH
#define SHIRO_PERMISSIONS_HH

#include <cstdint>

namespace shiro::permissions {

    enum class perms : uint64_t {
        // General permissions (<32)

        // Default channel permissions
        channel_console = 1LL << 31,

        // Command permissions (>32)
        cmd_announce = 1LL << 32,
        cmd_clear = 1LL << 33,
        cmd_clients = 1LL << 34,
        cmd_rtx = 1LL << 35
    };

}

#endif  // SHIRO_PERMISSIONS_HH
