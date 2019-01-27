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

#include "osu_reader.hh"

shiro::io::osu_reader::osu_reader(std::string data) {
    this->data = buffer(data);
}

std::vector<shiro::io::osu_packet> &shiro::io::osu_reader::parse() {
    while (this->data.can_read(7)) {
        this->packets.emplace_back(osu_packet(this->data));
    }

    return this->packets;
}
