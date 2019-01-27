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

#include "../../spectating/spectator_manager.hh"
#include "../../users/user_manager.hh"
#include "start_spectating_handler.hh"

void shiro::handler::spectating::start::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    int32_t target_id = in.data.read<int32_t>();

    std::shared_ptr<shiro::users::user> target_user = users::manager::get_user_by_id(target_id);

    if (target_user == nullptr)
        return;

    shiro::spectating::manager::start_spectating(user, target_user);

    if (user->hidden)
        return;

    io::osu_writer host_writer;
    host_writer.spectator_join_host(user->user_id);

    target_user->queue.enqueue(host_writer);

    io::osu_writer user_writer;
    user_writer.spectator_join_user(user->user_id);

    for (const std::shared_ptr<shiro::users::user> &fellow_spectator : shiro::spectating::manager::get_spectators(target_user)) {
        fellow_spectator->queue.enqueue(user_writer);
    }
}
