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

#ifndef SHIRO_BEATMAP_RANKED_STATUS_HH
#define SHIRO_BEATMAP_RANKED_STATUS_HH

#include <cstdint>

namespace shiro::beatmaps {

    enum class status : int32_t {
        unknown = -2,
        unsubmitted = -1,
        latest_pending = 0,
        needs_update = 1,
        ranked = 2,
        approved = 3,
        qualified = 4,
        loved = 5

    };

}

#endif // SHIRO_BEATMAP_RANKED_STATUS_HH
