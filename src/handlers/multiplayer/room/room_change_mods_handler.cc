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
#include "room_change_mods_handler.hh"

void shiro::handler::multiplayer::room::change_mods::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    uint32_t mods = in.data.read<uint32_t>();

    shiro::multiplayer::match_manager::iterate([user, &mods](io::layouts::multiplayer_match &match) {
        auto iterator = std::find(match.multi_slot_id.begin(), match.multi_slot_id.end(), user->user_id);

        if (iterator == match.multi_slot_id.end())
            return;

        if (match.multi_special_modes == 1) { // Free Mod
            if (match.host_id == user->user_id) {
                if (mods & (uint32_t) utils::mods::double_time) {
                    match.active_mods = (uint32_t) utils::mods::double_time;

                    if (mods & (uint32_t) utils::mods::nightcore)
                        match.active_mods |= (uint32_t) utils::mods::nightcore;
                } else if (mods & (uint32_t) utils::mods::half_time) {
                    match.active_mods = (uint32_t) utils::mods::half_time;
                } else {
                    match.active_mods = (uint32_t) utils::mods::none;
                }
            }

            ptrdiff_t index = std::distance(match.multi_slot_id.begin(), iterator);
            match.multi_slot_mods.at(index) = (mods & utils::free_mods);
        } else if (match.multi_special_modes == 0) { // Host sets global mods
            if (match.host_id != user->user_id)
                return;

            match.active_mods = mods;
        }

        match.send_update(true);
    });
}
