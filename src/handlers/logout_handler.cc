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

#include "../users/user_manager.hh"
#include "logout_handler.hh"

void shiro::handler::logout::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    if (!users::manager::is_online(user))
        return;

    users::manager::logout_user(user);

    if (user->hidden)
        return;

    io::layouts::user_quit quit;
    io::osu_writer writer;

    quit.user_id = user->user_id;
    quit.state = 0;

    writer.user_quit(quit);

    for (const std::shared_ptr<users::user> &online_user : users::manager::online_users.iterable()) {
        if (online_user->user_id == user->user_id || online_user->user_id == 1)
            continue;

        online_user->queue.enqueue(writer);
    }
}
