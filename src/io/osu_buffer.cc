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

#include "osu_buffer.hh"

void shiro::io::buffer::allocate(size_t amount) {
    for (size_t i = 0; i < amount; i++) {
        this->bytes.emplace_back((uint8_t) 0x0);
    }
}

void shiro::io::buffer::append(std::string data) {
    for (const char &c : data) {
        this->write<uint8_t>(c);
    }
}

void shiro::io::buffer::append(buffer &buf) {
    this->append(buf.serialize());
}

shiro::io::buffer::buffer(const buffer &buf) {
    for (uint8_t byte : buf.bytes) {
        this->write<uint8_t>(byte);
    }
}

shiro::io::buffer::buffer(std::string data) {
    for (const char &c : data) {
        this->write<uint8_t>(c);
    }
}

void shiro::io::buffer::write_string(std::string data) {
    this->append(data);
}

void shiro::io::buffer::write_array(std::vector<int32_t> data) {
    this->write<int16_t>(data.size());

    for (int32_t item : data) {
        this->write<int32_t>(item);
    }
}

std::string shiro::io::buffer::read_string() {
    if (this->read<uint8_t>() == 11) {
        int32_t total = 0;
        int32_t shift = 0;

        std::string result;
        uint8_t byte = this->read<uint8_t>();

        if ((byte & 0x80) == 0) {
            total |= ((byte & 0x7F) << shift);
        } else {
            bool end = false;

            do {
                if (shift)
                    byte = this->read<uint8_t>();

                total |= ((byte & 0x7F) << shift);

                if ((byte & 0x80) == 0)
                    end = true;

                shift += 7;
            } while (!end);
        }

        for (int32_t i = 0; i < total; i++) {
            result += this->read<uint8_t>();
        }

        return result;
    }

    return "";
}

std::vector<int32_t> shiro::io::buffer::read_array() {
    std::vector<int32_t> result;
    uint16_t size = this->read<uint16_t>();

    for (uint16_t i = 0; i < size; i++) {
        result.emplace_back(this->read<int32_t>());
    }

    return result;
}

std::string shiro::io::buffer::serialize() {
    std::stringstream stream;

    for (size_t i = 0; i < this->written_size; i++)
        stream << std::hex << this->bytes.at(i);

    return stream.str();
}

bool shiro::io::buffer::can_read(size_t amount) {
    return amount <= this->written_size - this->position;
}

bool shiro::io::buffer::is_empty() {
    return this->written_size == 0;
}

void shiro::io::buffer::clear() {
    this->bytes.clear();
    this->written_size = 0;
    this->position = 0;
}

size_t shiro::io::buffer::get_size() {
    return this->written_size;
}

