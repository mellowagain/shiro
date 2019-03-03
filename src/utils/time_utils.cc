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

#include <boost/lexical_cast.hpp>
#include <chrono>

#include "time_utils.hh"

int64_t shiro::utils::time::unix_epoch_ticks = 621355968000000000;

std::unordered_map<std::string, uint32_t> shiro::utils::time::duration_mapping = {
    { "s", 1 },
    { "min", 60 },
    { "h", 60 * 60 },
    { "d", 60 * 60 * 24 },
    { "w", 60 * 60 * 24 * 7 },
    { "m", 60 * 60 * 24 * 7 * 30 },
    //{ "y", 60 * 60 * 24 * 7 * 30 * 365 }
};

int64_t shiro::utils::time::get_current_time_ticks() {
    using ticks = std::chrono::duration<int64_t, std::ratio_multiply<std::ratio<100>, std::nano>>;

    std::chrono::time_point now = std::chrono::system_clock::now();
    int64_t ticks_since_epoch = std::chrono::duration_cast<ticks>(now.time_since_epoch()).count();
    ticks_since_epoch += unix_epoch_ticks;

    return ticks_since_epoch;
}

// This code is written in a ugly and disguising way
// It can be easily made prettier by using multiple methods
// However our method budget is tight which is why it wasn't implemented that way
// This should and must be changed at a later point.
std::optional<uint32_t> shiro::utils::time::parse_time_string(const std::string &input) {
    if (input.empty())
        return std::nullopt;

    const char &last_char = input.back();
    std::string raw_unit = "s";
    std::string raw_time = input;
    uint32_t time = 0;

    // Only parse the time suffix if we actually have one
    if (std::isalpha(last_char)) {
        // If the input is longer than 3, the suffix can be "min"
        if (input.length() > 3) {
            const std::string &suffix = input.substr(input.length() - 3);

            if (suffix == "min") {
                raw_unit = suffix;
                raw_time = input.substr(0, input.length() - 3);
            }
        }

        // If the above check didn't succeed, set the values now but with the last character
        if (raw_time.empty() || raw_unit.empty()) {
            raw_unit = last_char;
            raw_time = input.substr(0, input.length() - 1);
        }
    }

    // Remove non-digit characters from the raw_time
    // Currently uncommented as for the use case it should error instead of silently filtering them out
    // If however this is used for more than just the !silence command, this needs to be changed.
    //raw_time.erase(std::remove_if(raw_time.begin(), raw_time.end(), [](char c) {
    //    return !std::isdigit(c);
    //}), raw_time.end());

    try {
        time = boost::lexical_cast<uint32_t>(raw_time);
    } catch (const boost::bad_lexical_cast &ex) {
        return std::nullopt;
    }

    auto iterator = duration_mapping.find(raw_unit);

    if (iterator == duration_mapping.end())
        return std::nullopt;

    return time * iterator->second;
}
