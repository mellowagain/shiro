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

#ifndef SHIRO_OSU_BUFFER_HH
#define SHIRO_OSU_BUFFER_HH

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace shiro::io {

    class buffer {
    private:
        std::vector<uint8_t> bytes;

        size_t written_size = 0;
        size_t position = 0;

        void allocate(size_t amount);

    public:
        buffer() = default;
        buffer(const buffer &buf);
        explicit buffer(std::string data);

        void append(std::string data);
        void append(buffer &buf);

        template <typename t = uint8_t>
        void write(t data) {
            this->allocate(sizeof(t));
            uint8_t *data_arr = reinterpret_cast<uint8_t*>(&data);

            for (size_t i = 0; i < sizeof(t); i++)
                this->bytes.at(this->written_size++) = data_arr[i];
        }

        template <typename t = uint8_t>
        t read() {
            t data = *reinterpret_cast<t*>((uintptr_t)this->bytes.data() + this->position);
            this->position += sizeof(t);

            return data;
        }

        void write_string(std::string data);
        void write_array(std::vector<int32_t> data);

        std::string read_string();
        std::vector<int32_t> read_array();

        std::string serialize();

        bool can_read(size_t amount);
        bool is_empty();

        void clear();
        void seek(size_t position);
        void advance(size_t amount);

        size_t get_size();

    };

}

#endif //SHIRO_OSU_BUFFER_HH
