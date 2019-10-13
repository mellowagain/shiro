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
#include "../../../utils/slot_status.hh"
#include "match_start_handler.hh"

void shiro::handler::multiplayer::match::start::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    shiro::multiplayer::match_manager::iterate([user](io::layouts::multiplayer_match &match) -> bool {
        auto iterator = std::find(match.multi_slot_id.begin(), match.multi_slot_id.end(), user->user_id);

        if (iterator == match.multi_slot_id.end())
            return false;

        if (match.host_id != user->user_id)
            return true;

        io::osu_writer writer;
        writer.match_start(match, false);

        match.in_progress = true;

        for (size_t i = 0; i < match.multi_slot_id.size(); i++) {
            if (match.multi_slot_id.at(i) == -1)
                continue;

            match.multi_slot_status.at(i) = (uint8_t) utils::slot_status::playing;

            std::shared_ptr<users::user> lobby_user = users::manager::get_user_by_id(match.multi_slot_id.at(i));

            if (lobby_user == nullptr)
                continue;

            lobby_user->queue.enqueue(writer);
        }

        match.send_update(true);
        return true;
    });
}
