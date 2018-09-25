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

#include <ctime>

#include "../shiro.hh"
#include "index_page.hh"
#include "index_view.hh"

std::string shiro::views::index::get_view() {
    std::string view = std::string(reinterpret_cast<char*>(shiro_index_html), shiro_index_html_len);

    double difference = std::difftime(std::time(nullptr), start_time);
    std::time_t seconds((time_t) difference);
    std::tm *p = std::gmtime(&seconds);

    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "%i days %i hours %i minutes %i seconds", p->tm_yday, p->tm_hour, p->tm_min, p->tm_sec);

    size_t index = view.find("{{uptime}}");
    if (index == std::string::npos)
        return std::string();

    view.replace(index, 10, buffer);
    return view;
}
