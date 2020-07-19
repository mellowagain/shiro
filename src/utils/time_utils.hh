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

#ifndef SHIRO_TIME_UTILS_HH
#define SHIRO_TIME_UTILS_HH

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>

namespace shiro::utils::time {

    extern int64_t unix_epoch_ticks;

    extern std::unordered_map<std::string, uint32_t> duration_mapping;

    int64_t get_current_time_ticks();

    // Returns seconds of the parsed time string, e.g input: "15min" returns 900 (seconds)
    std::optional<uint32_t> parse_time_string(const std::string &input);

}

#endif //SHIRO_TIME_UTILS_HH
