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

#ifndef SHIRO_OSU_CLIENT_HH
#define SHIRO_OSU_CLIENT_HH

#include <string>
#include <vector>

#include "../thirdparty/enum.hh"
#include "../thirdparty/loguru.hh"
#include "../users/user.hh"

namespace shiro::utils::clients {

    BETTER_ENUM(osu_client, uint32_t,
        // Parsing was unable to be parsed or wasn't send at all
        unknown = 0,

        // Official osu! clients
        stable = 1,
        beta = 2,
        cutting_edge = 3,
        fallback = 4,
        dev = 5,
        public_test = 6,
        no_xna = 7,
        lazer = 8,
        tournament = 9,

        // Bots
        aschente = 10,

        // Third party osu! clients
        osu_fx = 11,
        banana_client = 12,
        tsuki = 13
    );

    osu_client parse_version(const std::string &client_version, const int32_t &client_build);

    // Checks whenever a client is a official osu! client
    // available for download on the official osu! website
    bool is_official(const osu_client &client);

    // Checks whenever a client is a widespread third party
    // client. These clients are *most* of the times based on
    // osu!fallback's leaked source code and may contain modifications.
    bool is_thirdparty(const osu_client &client);

    // Checks whenever a client is suspicious. This includes
    // official client versions which are *unable* for download
    // on the official osu! website, third party clients which are
    // known to have malicious features or if the client is unknown.
    bool is_suspicious(const osu_client &client);

    std::vector<std::shared_ptr<users::user>> get_users(const osu_client &client);

    std::string to_pretty_string(const osu_client &client);

}

#endif  // SHIRO_OSU_CLIENT_HH
