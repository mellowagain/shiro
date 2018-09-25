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

#include "user_quit.hh"

shiro::io::buffer shiro::io::layouts::user_quit::marshal() {
    io::buffer buf;

    buf.write<int32_t>(this->user_id);
    buf.write<uint8_t>(this->state);

    return buf;
}

void shiro::io::layouts::user_quit::unmarshal(shiro::io::buffer &buffer) {
    this->user_id = buffer.read<int32_t>();
    this->state = buffer.read<uint8_t>();
}

int32_t shiro::io::layouts::user_quit::get_size() {
    return 4 + 1;
}
