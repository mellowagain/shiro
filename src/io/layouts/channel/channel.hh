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

#ifndef SHIRO_CHANNEL_HH
#define SHIRO_CHANNEL_HH

#include "../serializable.hh"

namespace shiro::io::layouts {

    class channel : public serializable {
    public:
        channel() = default;
        channel(uint32_t id, bool auto_join, bool hidden, std::string name, std::string description, int32_t user_count, bool read_only, uint64_t permission);

        // Not written
        uint32_t id = 0;
        bool auto_join = false;
        bool hidden = false;
        bool read_only = false;
        uint64_t permission = 0;

        // Written
        std::string name = "";
        std::string description = "";
        int32_t user_count = 0;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;

        bool operator==(const channel &other_channel) const;
        bool operator!=(const channel &other_channel) const;

    };

}

namespace std {

    template <>
    struct hash<shiro::io::layouts::channel> {

        size_t operator()(const shiro::io::layouts::channel &channel) const;

    };

}

#endif //SHIRO_CHANNEL_HH
