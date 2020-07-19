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

#include <boost/algorithm/string.hpp>
#include <ctime>
#include <fstream>

#include "../utils/filesystem.hh"
#include "../shiro.hh"
#include "index_page.hh"
#include "index_view.hh"

void shiro::views::index::replace_hash(std::string &view) {
    static bool ran_once = false;

    if (!ran_once) {
        ran_once = true;

        if (fs::exists("commit.txt")) {
            std::ifstream stream("commit.txt");

            std::stringstream buffer;
            buffer << stream.rdbuf();

            commit = buffer.str();
        }
    }

    boost::replace_all(view, "{{hash}}", commit);
}

void shiro::views::index::replace_time(std::string &view) {
    double difference = std::difftime(std::time(nullptr), start_time);
    std::time_t seconds((std::time_t) difference);
    std::tm *p = std::gmtime(&seconds);

    int days = p->tm_yday;
    int hours = p->tm_hour;
    int minutes = p->tm_min;
    int secs = p->tm_sec;

    char buffer[64];
    std::snprintf(
            buffer,
            sizeof(buffer),
            "%i %s %i %s %i %s %i %s",
            days, days == 1 ? "day" : "days",
            hours, hours == 1 ? "hour" : "hours",
            minutes, minutes == 1 ? "minute" : "minutes",
            secs, secs == 1 ? "second" : "seconds"
    );

    boost::replace_all(view, "{{uptime}}", buffer);
}

std::string shiro::views::index::get_view() {
    std::string view = std::string(reinterpret_cast<char*>(shiro_index_html), shiro_index_html_len);

    // Templates are overrated anyway
    replace_hash(view);
    replace_time(view);

    return view;
}
