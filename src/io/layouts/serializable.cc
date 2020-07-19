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

#include "serializable.hh"
#include "../../utils/osu_string.hh"

shiro::io::serializable::serializable(uint8_t data) {
    this->type = data_type::byte;
    this->data_byte = data;
}

shiro::io::serializable::serializable(int16_t data) {
    this->type = data_type::int16;
    this->data_short = data;
}

shiro::io::serializable::serializable(uint16_t data) {
    this->type = data_type::uint16;
    this->data_ushort = data;
}

shiro::io::serializable::serializable(int32_t data) {
    this->type = data_type::int32;
    this->data_int = data;
}

shiro::io::serializable::serializable(uint32_t data) {
    this->type = data_type::uint32;
    this->data_uint = data;
}

shiro::io::serializable::serializable(int64_t data) {
    this->type = data_type::int64;
    this->data_long = data;
}

shiro::io::serializable::serializable(uint64_t data) {
    this->type = data_type::uint64;
    this->data_ulong = data;
}

shiro::io::serializable::serializable(float data) {
    this->type = data_type::flt;
    this->data_float = data;
}

shiro::io::serializable::serializable(double data) {
    this->type = data_type::dble;
    this->data_double = data;
}

shiro::io::serializable::serializable(std::string data) {
    this->type = data_type::string;
    this->data_string = std::move(data);
}

shiro::io::serializable::serializable(std::vector<int32_t> data) {
    this->type = data_type::array;
    this->data_array = std::move(data);
}

int32_t shiro::io::serializable::get_size() {
    return (int32_t) this->marshal().get_size();
}

shiro::io::buffer shiro::io::serializable::marshal() {
    buffer buf;

    switch (this->type) {
    case data_type::byte:
        buf.write<uint8_t>(this->data_byte);
        break;
    case data_type::int16:
        buf.write<int16_t>(this->data_short);
        break;
    case data_type::uint16:
        buf.write<uint16_t>(this->data_ushort);
        break;
    case data_type::int32:
        buf.write<int32_t>(this->data_int);
        break;
    case data_type::uint32:
        buf.write<uint32_t>(this->data_uint);
        break;
    case data_type::int64:
        buf.write<int64_t>(this->data_long);
        break;
    case data_type::uint64:
        buf.write<uint64_t>(this->data_ulong);
        break;
    case data_type::flt:
        buf.write<float>(this->data_float);
        break;
    case data_type::dble:
        buf.write<double>(this->data_double);
        break;
    case data_type::string: {
        buf.write_string(utils::osu_string(this->data_string));
        break;
    }
    case data_type::array:
        buf.write_array(this->data_array);
        break;
    }

    return buf;
}

void shiro::io::serializable::unmarshal(shiro::io::buffer &buffer) {
    // That's not that easy...

    // Reading values directly from buffer
    // will be a better idea
}
