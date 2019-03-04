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

#include <vector>

#include "../../users/user_manager.hh"
#include "user_stats_request_handler.hh"

void shiro::handler::stats::request_all::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    std::vector<int32_t> requested_users = in.data.read_array();

    for (int32_t requested_user : requested_users) {
        if (!users::manager::is_online(requested_user))
            continue;

        std::shared_ptr<users::user> target_user = users::manager::get_user_by_id(requested_user);

        if (target_user == nullptr || target_user->hidden)
            continue;

        if (target_user->user_id != 1)
            out.user_stats(target_user->stats);

        out.user_presence(target_user->presence);
    }

    std::vector<int32_t> online_users;
    online_users.reserve(users::manager::online_users.size());

    for (const std::shared_ptr<users::user> &online_user : users::manager::online_users) {
        if (online_user->hidden)
            continue;

        online_users.emplace_back(online_user->user_id);
    }

    out.users_list(online_users);
}
