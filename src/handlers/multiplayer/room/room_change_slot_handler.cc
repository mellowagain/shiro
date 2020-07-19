/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2020 Marc3842h, czapek
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
#include "../../../utils/slot_status.hh"
#include "room_change_slot_handler.hh"

void shiro::handler::multiplayer::room::change_slot::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    int32_t slot_id = in.data.read<int32_t>();

    if (slot_id >= 16)
        return;

    shiro::multiplayer::match_manager::iterate([user, slot_id](io::layouts::multiplayer_match &match) -> bool {
        auto iterator = std::find(match.multi_slot_id.begin(), match.multi_slot_id.end(), user->user_id);

        if (iterator == match.multi_slot_id.end())
            return false;

        if (match.multi_slot_status.at(slot_id) != (uint8_t) utils::slot_status::open)
            return true;

        ptrdiff_t index = std::distance(match.multi_slot_id.begin(), iterator);

        match.multi_slot_id.at(slot_id) = match.multi_slot_id.at(index);
        match.multi_slot_team.at(slot_id) = match.multi_slot_team.at(index);
        match.multi_slot_mods.at(slot_id) = match.multi_slot_mods.at(index);
        match.multi_slot_status.at(slot_id) = match.multi_slot_status.at(index);

        match.multi_slot_id.at(index) = -1;
        match.multi_slot_team.at(index) = 0;
        match.multi_slot_mods.at(index) = 0;
        match.multi_slot_status.at(index) = (uint8_t) utils::slot_status::open;

        match.send_update(true);
        return true;
    });
}
