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

#include <utility>

#include "../../../utils/osu_string.hh"
#include "channel.hh"

shiro::io::layouts::channel::channel(uint32_t id, bool auto_join, bool hidden, std::string name, std::string description, int32_t user_count, bool read_only, uint64_t permission)
        : id(id)
        , auto_join(auto_join)
        , hidden(hidden)
        , read_only(read_only)
        , permission(permission)
        , name(std::move(name))
        , description(std::move(description))
        , user_count(user_count) {
    // Initialized in initializer list
}

shiro::io::buffer shiro::io::layouts::channel::marshal() {
    std::string name = utils::osu_string(this->name);
    std::string description = utils::osu_string(this->description);

    buffer buf;

    buf.write_string(name);
    buf.write_string(description);
    buf.write<int32_t>(this->user_count);

    return buf;
}

void shiro::io::layouts::channel::unmarshal(shiro::io::buffer &buffer) {
    this->name = buffer.read_string();
    this->description = buffer.read_string();
    this->user_count = buffer.read<int32_t>();
}

int32_t shiro::io::layouts::channel::get_size() {
    return (int32_t) this->marshal().get_size();
}

bool shiro::io::layouts::channel::operator==(const shiro::io::layouts::channel &other_channel) const {
    return this->name == other_channel.name && this->description == other_channel.description;
}

bool shiro::io::layouts::channel::operator!=(const shiro::io::layouts::channel &other_channel) const {
    return !operator==(other_channel);
}

size_t std::hash<shiro::io::layouts::channel>::operator()(const shiro::io::layouts::channel &channel) const {
    size_t id_hash = std::hash<uint32_t>()(channel.id);
    size_t name_hash = std::hash<std::string>()(channel.name);
    size_t description_hash = std::hash<std::string>()(channel.description);

    return ((id_hash ^ (name_hash << 1)) >> 1) ^ (description_hash << 1);
}
