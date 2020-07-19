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

#include "user_status.hh"

shiro::io::buffer shiro::io::layouts::user_status::marshal() {
    buffer buf;

    return buf;
}

void shiro::io::layouts::user_status::unmarshal(shiro::io::buffer &data) {
    this->activity = data.read<uint8_t>();
    this->activity_desc = data.read_string();

    this->beatmap_checksum = data.read_string();

    this->current_mods = data.read<uint32_t>();
    this->play_mode = data.read<uint8_t>();

    this->beatmap_id = data.read<int32_t>();
}

int32_t shiro::io::layouts::user_status::get_size() {
    return (int32_t) this->marshal().get_size();
}
