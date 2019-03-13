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
#include "room_join_handler.hh"

void shiro::handler::multiplayer::room::join::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    io::layouts::multiplayer_join payload = in.unmarshal<io::layouts::multiplayer_join>();

    std::optional<io::layouts::multiplayer_match> joined_match = shiro::multiplayer::match_manager::join_match(payload, std::move(user));

    if (!joined_match.has_value()) {
        out.match_join_fail();
        return;
    }

    out.match_join_success(joined_match.value());
}
