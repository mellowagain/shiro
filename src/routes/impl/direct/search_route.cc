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
#include "search_route.hh"

void shiro::routes::direct::search::handle(const crow::request &request, crow::response &response) {
    response.set_header("Content-Type", "text/plain; charset=UTF-8");
    response.set_header("cho-server", "shiro (https://github.com/Marc3842h/shiro)");

    std::unordered_map<std::string, std::string> params = request.url_params.get_all();

    // Remove username from the request so the requesting user stays anonymous
    if (params.find("u") != params.end())
        params.erase("u");

    // Remove password hash from the request so no credentials are leaked
    if (params.find("h") != params.end())
        params.erase("h");

    // Send the origin server so our partner server can identify us
    params.insert(std::make_pair("cho-server", "shiro"));

    std::string url = config::bancho::direct_base_url + "/web/osu-search.php?";

    for (const auto &[key, value] : params) {
        url += (key + "=" + value + "&");
    }

    // Remove the last char (which will be a &)
    url = url.substr(0, url.length() - 1);

    LOG_F(INFO, "Reverse Proxy | Streaming request from osu!direct search from response: %s", url.c_str());

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
