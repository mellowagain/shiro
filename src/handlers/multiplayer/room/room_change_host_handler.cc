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

#include "../../../multiplayer/match_manager.hh"
#include "../../../users/user_manager.hh"
#include "room_change_host_handler.hh"

void shiro::handler::multiplayer::room::change_host::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    int32_t slot_id = in.data.read<int32_t>();

    if (slot_id >= 16)
        return;

    shiro::multiplayer::match_manager::iterate([&user, slot_id](io::layouts::multiplayer_match &match) {
        auto iterator = std::find(match.multi_slot_id.begin(), match.multi_slot_id.end(), user->user_id);

        if (iterator == match.multi_slot_id.end())
            return;

        if (match.host_id != user->user_id)
            return;

        int32_t new_host_id = match.multi_slot_id.at(slot_id);

        if (new_host_id == -1)
            return;

        match.host_id = new_host_id;
        match.send_update(true);

        std::shared_ptr<users::user> new_host = users::manager::get_user_by_id(new_host_id);

        if (new_host == nullptr)
            return;

        io::osu_writer writer;
        writer.match_transfer_host();

        new_host->queue.enqueue(writer);
    });
}
