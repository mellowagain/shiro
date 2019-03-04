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

#include "../../../config/bancho_file.hh"
#include "../../../thirdparty/loguru.hh"
#include "../../../utils/curler.hh"
#include "download_route.hh"

void shiro::routes::direct::download::handle(const crow::request &request, crow::response &response, std::string args) {
    response.set_header("Content-Type", "text/plain; charset=UTF-8");
    response.set_header("cho-server", "shiro (https://github.com/Marc3842h/shiro)");

    const char &last_char = args.back();
    bool no_video = false;

    if (last_char == 'n')
        no_video = true;

    int32_t id = 0;

    try {
        id = boost::lexical_cast<int32_t>(no_video ? args.substr(0, args.length() - 1) : args);
    } catch (boost::bad_lexical_cast &ex) {
        response.code = 400;
        response.end("Invalid beatmap id");
        return;
    }

    std::string url = config::bancho::direct_mirror_url + "/d/" + std::to_string(id) + "?";

    if (no_video)
        url += "novideo=yes&";

    // Send the origin server so our partner server can identify us
    url += "cho-server=shiro";

    auto [success, output] = utils::curl::get(url);

    // If we didn't succeed, let the osu! client do all the work
    if (!success) {
        response.set_header("Location", url);
        response.set_header("Cache-Control", "no-cache");
        response.set_header("Pragma", "no-cache");
        response.code = 302;
        response.end();

        LOG_F(WARNING, "Unable to contact beatmap mirror, asking client to contact it.");
        return;
    }

    response.code = 200;
    response.end(output);
}
