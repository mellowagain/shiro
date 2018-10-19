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

#include <cstring>
#include <utility>

#include "../database/tables/channel_table.hh"
#include "../thirdparty/loguru.hh"
#include "../shiro.hh"
#include "channel_manager.hh"

std::unordered_map<shiro::io::layouts::channel, std::vector<std::shared_ptr<shiro::users::user>>> shiro::channels::manager::channels;

void shiro::channels::manager::init() {
    if (!channels.empty())
        channels.clear();

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::channels channel_table {};

    auto result = db(select(all_of(channel_table)).from(channel_table).where(channel_table.name == "#announce"));
    bool empty = is_query_empty(result);

    if (empty) {
        db(insert_into(channel_table).set(
                channel_table.name = "#announce",
                channel_table.description = "",
                channel_table.auto_join = true,
                channel_table.hidden = false
        ));
    }

    result = db(select(all_of(channel_table)).from(channel_table).where(channel_table.name == "#lobby"));
    empty = is_query_empty(result);

    if (empty) {
        db(insert_into(channel_table).set(
                channel_table.name = "#lobby",
                channel_table.description = "",
                channel_table.auto_join = false,
                channel_table.hidden = true
        ));
    }

    result = db(select(all_of(channel_table)).from(channel_table).unconditionally());
    empty = is_query_empty(result);

    if (empty)
        return;

    for (const auto &row : result) {
        std::string name = row.name;

        if (name.at(0) != '#') {
            LOG_F(WARNING, "Channel name of channel id %i doesn't start with #, fixing (%s -> %s).", (int) row.id, name.c_str(), ("#" + name).c_str());
            name.insert(0, "#");

            db(update(channel_table).set(
                    channel_table.name = name
            ).where(channel_table.id == row.id));
        }

        channels.insert(std::make_pair<io::layouts::channel, std::vector<std::shared_ptr<users::user>>>(
                io::layouts::channel(row.id, row.auto_join, row.hidden, name, row.description, 0),
                std::vector<std::shared_ptr<users::user>>()
        ));
    }
}

void shiro::channels::manager::write_channels(shiro::io::osu_writer &buf, std::shared_ptr<shiro::users::user> user, bool first) {
    for (auto &[channel, users] : channels) {
        if (channel.hidden)
            continue;

        shiro::io::layouts::channel channel_layout;
        channel_layout.id = channel.id;
        channel_layout.auto_join = channel.auto_join;
        channel_layout.name = channel.name;
        channel_layout.description = channel.description;
        channel_layout.user_count = users.size();

        buf.channel_available(channel_layout);

        if (channel.auto_join && first) {
            buf.channel_join(channel.name);
            users.emplace_back(user);
        }
    }
}

void shiro::channels::manager::join_channel(uint32_t channel_id, std::shared_ptr<shiro::users::user> user) {
    if (in_channel(channel_id, user))
        leave_channel(channel_id, std::move(user));

    for (auto &[channel, users] : channels) {
        if (channel.id == channel_id) {
            users.emplace_back(user);
            break;
        }
    }
}

void shiro::channels::manager::leave_channel(uint32_t channel_id, std::shared_ptr<shiro::users::user> user) {
    if (!in_channel(channel_id, user))
        return;

    for (auto &[channel, users] : channels) {
        if (channel.id == channel_id) {
            auto iterator = std::find(users.begin(), users.end(), user);

            if (iterator == users.end())
                return;

            ptrdiff_t index = std::distance(users.begin(), iterator);
            users.erase(users.begin() + index);
            break;
        }
    }
}

bool shiro::channels::manager::in_channel(uint32_t channel_id, const std::shared_ptr<shiro::users::user> &user) {
    for (const auto &[channel, users] : channels) {
        if (channel.id == channel_id)
            return std::find(users.begin(), users.end(), user) != users.end();
    }

    return false;
}

std::vector<std::shared_ptr<shiro::users::user>> shiro::channels::manager::get_users_in_channel(const std::string &channel_name) {
    for (const auto &[channel, users] : channels) {
        if (channel.name == channel_name)
            return users;
    }

    return {};
}

uint32_t shiro::channels::manager::get_channel_id(const std::string &channel_name) {
    for (const auto &[channel, _] : channels) {
        if (channel.name == channel_name)
            return channel.id;
    }

    return 0;
}
