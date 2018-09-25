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

#include <algorithm>

#include "escaper.hh"

std::string shiro::utils::escaper::make_safe(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);

    input.erase(std::remove_if(input.begin(), input.end(), [](char c) {
        return !std::isalnum(c) && !std::isspace(c);
    }), input.end());

    std::replace(input.begin(), input.end(), ' ', '_');
    return input;
}
