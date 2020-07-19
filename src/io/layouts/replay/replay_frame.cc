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

#include "replay_frame.hh"

shiro::io::buffer shiro::io::layouts::replay_frame::marshal() {
    buffer buf;

    buf.write<uint8_t>(this->button_state);
    buf.write<uint8_t>(this->button_type);

    buf.write<float>(this->mouse_x);
    buf.write<float>(this->mouse_y);

    buf.write<int32_t>(this->time);

    return buf;
}

void shiro::io::layouts::replay_frame::unmarshal(shiro::io::buffer &buffer) {
    this->button_state = buffer.read<uint8_t>();
    this->button_type = buffer.read<uint8_t>();

    this->mouse_x = buffer.read<float>();
    this->mouse_y = buffer.read<float>();

    this->time = buffer.read<int32_t>();
}

int32_t shiro::io::layouts::replay_frame::get_size() {
    return this->marshal().get_size();
}
