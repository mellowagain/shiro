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

#include "../../config/direct_file.hh"
#include "../../utils/curler.hh"
#include "emulation.hh"

std::tuple<bool, std::string> shiro::direct::emulation::search(std::unordered_map<std::string, std::string> parameters) {
    // Remove username from the request so the requesting user stays anonymous
    if (parameters.find("u") != parameters.end())
        parameters.erase("u");

    // Remove password hash from the request so no credentials are leaked
    if (parameters.find("h") != parameters.end())
        parameters.erase("h");

    std::string url = config::direct::base_url + "/web/osu-search.php?";

    for (const auto &[key, value] : parameters) {
        url.append(key).append("=").append(utils::curl::escape_url(value)).append("&");
    }

    // Remove the last char (which will be a &)
    url = url.substr(0, url.length() - 1);

    return utils::curl::get_direct(url);
}

std::tuple<bool, std::string> shiro::direct::emulation::download(int32_t beatmap_id, bool no_video) {
    std::string url = config::direct::mirror_url + "/d/" + std::to_string(beatmap_id);

    if (no_video)
        url.append("?novideo=yes");

    return utils::curl::get_direct(url);
}

const std::string shiro::direct::emulation::name() const {
    return "Emulation";
}
