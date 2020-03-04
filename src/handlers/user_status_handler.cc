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

#include "../io/layouts/user/user_status.hh"
#include "user_status_handler.hh"

void shiro::handler::user_status::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    io::layouts::user_status status = in.unmarshal<io::layouts::user_status>();
    bool changed_mode = user->stats.play_mode != status.play_mode;

    #if defined(SEPARATE_RX_LEADERBOARDS)
        bool changed_relax = ((status.current_mods & (uint32_t) utils::mods::relax) &&
                !(user->stats.current_mods & (uint32_t) utils::mods::relax)) ||
                (!(status.current_mods & (uint32_t) utils::mods::relax) &&
                (user->stats.current_mods & (uint32_t) utils::mods::relax));

        if (status.current_mods & (uint32_t) utils::mods::relax)
            status.activity_desc.insert(0, "[Relax] ");
    #endif

    #if defined(SEPARATE_AP_LEADERBOARDS)
        if (status.current_mods & (uint32_t) utils::mods::auto_pilot)
            status.activity_desc.insert(0, "[Auto Pilot] ");
    #endif

    user->status = status;

    user->stats.current_mods = status.current_mods;
    user->stats.activity = status.activity;
    user->stats.activity_desc = status.activity_desc;
    user->stats.beatmap_id = status.beatmap_id;
    user->stats.beatmap_checksum = status.beatmap_checksum;
    user->stats.play_mode = status.play_mode;

    if (!changed_mode)
        return;

    user->update();
}
