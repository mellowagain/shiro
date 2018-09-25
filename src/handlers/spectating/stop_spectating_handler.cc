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

#include "../../spectating/spectator_manager.hh"
#include "stop_spectating_handler.hh"

void shiro::handler::spectating::stop::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    if (!shiro::spectating::manager::is_spectating(user))
        return;

    std::shared_ptr<users::user> host = shiro::spectating::manager::get_host(user);

    if (host == nullptr)
        return;

    shiro::spectating::manager::stop_spectating(user);

    if (user->hidden)
        return;

    io::osu_writer host_writer;
    host_writer.spectator_left_host(user->user_id);

    host->queue.enqueue(host_writer);

    io::osu_writer user_writer;
    user_writer.spectator_left_user(user->user_id);

    for (const std::shared_ptr<shiro::users::user> &fellow_spectator : shiro::spectating::manager::get_spectators(host)) {
        fellow_spectator->queue.enqueue(user_writer);
    }
}
