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

#include "play_mode.hh"

std::string shiro::utils::play_mode_to_string(shiro::utils::play_mode mode, bool long_name) {
    switch (mode) {
        case play_mode::taiko:
            return long_name ? "osu!taiko" : "taiko";
        case play_mode::fruits:
            return long_name ? "osu!catch" : "ctb";
        case play_mode::mania:
            return long_name ? "osu!mania" : "mania";
        case play_mode::standard:
        default:
            return long_name ? "osu!" : "std";
    }
}
