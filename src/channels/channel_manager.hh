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

#ifndef SHIRO_CHANNEL_MANAGER_HH
#define SHIRO_CHANNEL_MANAGER_HH

#include <shared_mutex>
#include <unordered_map>

#include "../io/layouts/channel/channel.hh"
#include "../io/osu_writer.hh"
#include "../users/user.hh"

namespace shiro::channels::manager {

    extern std::unordered_map<io::layouts::channel, std::vector<std::shared_ptr<users::user>>> channels;
    extern std::shared_timed_mutex mutex;
    extern std::vector<std::pair<uint32_t, io::layouts::channel>> auto_join_channels;

    void init();

    void write_channels(io::osu_writer &buffer, std::shared_ptr<shiro::users::user> user);

    void auto_join(io::osu_writer &buffer, std::shared_ptr<shiro::users::user> user);

    bool join_channel(uint32_t channel_id, std::shared_ptr<users::user> user);

    bool leave_channel(uint32_t channel_id, std::shared_ptr<users::user> user);

    bool in_channel(uint32_t channel_id, std::shared_ptr<users::user> user);

    std::vector<std::shared_ptr<users::user>> get_users_in_channel(const std::string &channel_name);

    uint32_t get_channel_id(const std::string &channel_name);

    void insert_if_not_exists(std::string name, std::string description, bool auto_join, bool hidden, bool read_only, uint64_t permission);

    bool has_permissions(std::shared_ptr<users::user> user, uint64_t perms);

    bool is_read_only(uint32_t channel_id);

    bool is_read_only(std::string channel_name);

}

#endif //SHIRO_CHANNEL_MANAGER_HH
