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

#include "../../io/layouts/replay/spectate_frames.hh"
#include "../../spectating/spectator_manager.hh"
#include "spectator_frames_handler.hh"

#include "../../thirdparty/loguru.hh"

void shiro::handler::spectating::frames::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    auto spectators = shiro::spectating::manager::get_spectators(user);

    if (spectators.empty())
        return;

    io::layouts::spectate_frames frames = in.unmarshal<io::layouts::spectate_frames>();

    io::osu_writer writer;
    writer.spectate_frames(frames);

    for (const std::shared_ptr<users::user> &spectator : spectators) {
        spectator->queue.enqueue(writer);
    }
}
