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

#include <chrono>

#include "time_utils.hh"

constexpr int64_t unix_epoch_ticks = 621355968000000000;

int64_t shiro::utils::time::get_current_time_ticks() {
    using ticks = std::chrono::duration<int64_t, std::ratio_multiply<std::ratio<100>, std::nano>>;

    std::chrono::time_point now = std::chrono::system_clock::now();
    int64_t ticks_since_epoch = std::chrono::duration_cast<ticks>(now.time_since_epoch()).count();
    ticks_since_epoch += unix_epoch_ticks;

    return ticks_since_epoch;
}
