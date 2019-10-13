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
#include "room_change_password_handler.hh"

void shiro::handler::multiplayer::room::change_password::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    io::layouts::multiplayer_match match = in.unmarshal<io::layouts::multiplayer_match>();

    shiro::multiplayer::match_manager::iterate([&user, match](io::layouts::multiplayer_match &global_match) -> bool {
        if (global_match.match_id != match.match_id)
            return false;

        if (global_match.game_password == match.game_password)
            return true;

        auto iterator = std::find(global_match.multi_slot_id.begin(), global_match.multi_slot_id.end(), user->user_id);

        if (iterator == global_match.multi_slot_id.end())
            return true;

        if (global_match.host_id != user->user_id)
            return true;

        global_match.game_password = match.game_password;
        global_match.send_update(true);
        return true;
    });
}
