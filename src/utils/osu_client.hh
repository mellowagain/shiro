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

#include <cstdint>
#include <string>
#include <algorithm>
#include "../thirdparty/loguru.hh"

namespace shiro::utils {

    enum class osu_client : uint32_t {
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

        // Third party osu! clients
        osu_fx = 10,
        banana_client = 11,
        tsuki = 12,

    };

    // This method only parses the client version of the official osu!
    // clients. Third party clients are detected at a later point after
    // the login has been successfully processed.
    inline osu_client parse_client(const std::string &client_version, const int32_t &client_build) {
        if (client_build <= 20160403)
            return osu_client::fallback;

        // osu!lazer contains two dots in their version number, unlike stable clients
        if (client_version.find('.', client_version.find('.') + 1) != std::string::npos)
            return osu_client::lazer;

        if (client_version.find("beta") != std::string::npos)
            return osu_client::beta;

        if (client_version.find("cuttingedge") != std::string::npos)
            return osu_client::cutting_edge;

        if (client_version.find("tourney") != std::string::npos)
            return osu_client::tournament;

        // All checks below are suspicious as they can't be obtained legit from the osu! website
        if (client_version.find("dev") != std::string::npos)
            return osu_client::dev;

        if (client_version.find("public_test") != std::string::npos)
            return osu_client::public_test;

        if (client_version.find("noxna") != std::string::npos)
            return osu_client::no_xna;

        std::string subversion = client_version.substr(1);

        if (subversion.find('.') != std::string::npos)
            subversion.erase(subversion.find('.'));

        subversion.erase(std::remove_if(subversion.begin(), subversion.end(), [](char c) {
            return std::isdigit(c);
        }), subversion.end());

        if (subversion.empty())
            return osu_client::stable;

        LOG_F(INFO, "Tried to process osu! version with unknown subversion: %s (%i -> '%s`)", client_version.c_str(), client_build, subversion.c_str());

        return osu_client::unknown;
    }

}

#endif  // SHIRO_OSU_CLIENT_HH
