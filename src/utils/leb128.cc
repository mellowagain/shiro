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

#include "leb128.hh"

uint64_t shiro::utils::leb128::read_leb128(shiro::io::buffer &buffer) {
    uint64_t total = 0;
    int shift = 0;

    uint8_t byte = buffer.read<uint8_t>();

    if ((byte & 0x80) == 0) {
        total |= ((byte & 0x7F) << shift);
    } else {
        bool end = false;

        do {
            if (shift) {
                byte = buffer.read<uint8_t>();
            }

            total |= ((byte & 0x7F) << shift);

            if ((byte & 0x80) == 0)
                end = true;

            shift += 7;
        } while (!end);
    }

    return total;
}

void shiro::utils::leb128::write_leb128(shiro::io::buffer &buffer, uint64_t value) {
    if (value == 0) {
        buffer.write<uint8_t>(static_cast<std::uint8_t>('\0'));
    } else {
        int len = 0;
        std::vector<uint8_t> bytes;

        do {
            bytes.push_back(0x0);

            bytes.at(len) = value & 0x7f;

            if ((value >>= 7) != 0)
                bytes.at(len) |= 0x80;

            len++;
        } while (value > 0);

        for (uint8_t &byte : bytes) {
            buffer.write<uint8_t>(byte);
        }
    }
}
