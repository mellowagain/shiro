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
#include "../../../utils/match_team_type.hh"
#include "../../../utils/slot_status.hh"
#include "room_change_settings_handler.hh"

void shiro::handler::multiplayer::room::change_settings::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    io::layouts::multiplayer_match match = in.unmarshal<io::layouts::multiplayer_match>();

    shiro::multiplayer::match_manager::iterate([&user, match](io::layouts::multiplayer_match &global_match) -> bool {
        if (match.match_id != global_match.match_id)
            return false;

        if (global_match.host_id != user->user_id)
            return true;

        bool changed_beatmap = global_match.beatmap_id != match.beatmap_id;
        bool changed_win_condition = global_match.multi_win_condition != match.multi_win_condition;
        bool changed_team_type = global_match.multi_team_type != match.multi_team_type;

        global_match.multi_team_type = match.multi_team_type;
        global_match.multi_win_condition = match.multi_win_condition;
        global_match.multi_special_modes = match.multi_special_modes;

        global_match.game_name = match.game_name;

        global_match.beatmap_id = match.beatmap_id;
        global_match.beatmap_name = match.beatmap_name;
        global_match.beatmap_checksum = match.beatmap_checksum;

        if (global_match.multi_special_modes == 0) {
            std::fill(global_match.multi_slot_mods.begin(), global_match.multi_slot_mods.end(), 0);
        } else if (global_match.multi_special_modes == 1) {
            for (size_t i = 0; i < global_match.multi_slot_mods.size(); i++) {
                if (global_match.multi_slot_id.at(i) == -1)
                    continue;

                global_match.multi_slot_mods.at(i) = global_match.active_mods & utils::free_mods;
            }

            global_match.active_mods &= ~utils::free_mods;
        }

        if (utils::is_team(global_match.multi_team_type)) {
            for (size_t i = 0; i < global_match.multi_slot_id.size(); i++) {
                if (global_match.multi_slot_id.at(i) == -1)
                    continue;

                global_match.multi_slot_team.at(i) = i % 2 + 1;
            }
        } else {
            std::fill(global_match.multi_slot_team.begin(), global_match.multi_slot_team.end(), 0);
        }

        if (changed_beatmap || changed_win_condition || changed_team_type) {
            for (size_t i = 0; i < global_match.multi_slot_id.size(); i++) {
                if (global_match.multi_slot_id.at(i) == -1)
                    continue;

                if (global_match.multi_slot_status.at(i) != (uint8_t) utils::slot_status::ready)
                    continue;

                global_match.multi_slot_status.at(i) = (uint8_t) utils::slot_status::not_ready;
            }
        }

        global_match.send_update(true);
        return true;
    });
}
