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

#include "../../../utils/osu_string.hh"
#include "user_presence.hh"

shiro::io::buffer shiro::io::layouts::user_presence::marshal() {
    std::string username = utils::osu_string(this->username);

    buffer buf;

    buf.write<int32_t>(this->user_id);
    buf.write_string(username);

    buf.write<uint8_t>(this->time_zone);
    buf.write<uint8_t>(this->country_id);

    buf.write<uint8_t>(this->permissions);

    buf.write<float>(this->longitude);
    buf.write<float>(this->latitude);

    buf.write<int32_t>(this->rank);

    return buf;
}

void shiro::io::layouts::user_presence::unmarshal(shiro::io::buffer &buffer) {
    this->user_id = buffer.read<int32_t>();
    this->username = buffer.read_string();

    this->time_zone = buffer.read<uint8_t>();
    this->country_id = buffer.read<uint8_t>();

    this->permissions = buffer.read<uint8_t>();

    this->longitude = buffer.read<float>();
    this->latitude = buffer.read<float>();

    this->rank = buffer.read<int32_t>();
}

int32_t shiro::io::layouts::user_presence::get_size() {
    return (int32_t) this->marshal().get_size();
}
