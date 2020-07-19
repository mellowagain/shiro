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

#ifndef SHIRO_CLIENT_SIDE_FLAGS_HH
#define SHIRO_CLIENT_SIDE_FLAGS_HH

#include <cstdint>

#include "../thirdparty/enum.hh"

namespace shiro::utils {

    // Up-to-date with osu!stable 20190401.2 (1st of April 2019)
    // Values provided by Cyuubi, thanks!

    BETTER_ENUM(
            client_side_flags, int32_t,
            cli_argument = 1 << 0,
            console_window = 1 << 1,
            interacting_process = 1 << 2,
            hq_osu_loaded = 1 << 3,
            existing_hq_osu_file = 1 << 4,
            debugger_override = 1 << 5,
            sdl2_loaded = 1 << 6,
            eay32_loaded = 1 << 7
    )

}

#endif  // SHIRO_CLIENT_SIDE_FLAGS_HH
