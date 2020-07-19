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

#include "../io/osu_buffer.hh"
#include "leb128.hh"
#include "osu_string.hh"

std::string shiro::utils::osu_string(std::string value, bool nullable) {
    io::buffer buffer;

    if (value.empty()) {
        if (nullable) {
            buffer.write<uint8_t>(0);
        } else {
            buffer.write<uint8_t>(11);
            buffer.write<uint8_t>(0);
        }
    } else {
        buffer.write<uint8_t>(11);
        utils::leb128::write_leb128(buffer, value.size());
        buffer.write_string(value);
    }

    return buffer.serialize();
}
