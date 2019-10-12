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
#include "room_change_settings_handler.hh"

void shiro::handler::multiplayer::room::change_settings::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    io::layouts::multiplayer_match match = in.unmarshal<io::layouts::multiplayer_match>();

    shiro::multiplayer::match_manager::iterate([&user, match](io::layouts::multiplayer_match &global_match) {
        if (match.match_id != global_match.match_id)
            return;

        if (global_match.host_id != user->user_id)
            return;

        global_match.multi_team_type = match.multi_team_type;
        global_match.multi_win_condition = match.multi_win_condition;
        global_match.multi_special_modes = match.multi_special_modes;

        global_match.game_name = match.game_name;

        global_match.beatmap_id = match.beatmap_id;
        global_match.beatmap_name = match.beatmap_name;
        global_match.beatmap_checksum = match.beatmap_checksum;

        global_match.send_update(true);
    });
}
