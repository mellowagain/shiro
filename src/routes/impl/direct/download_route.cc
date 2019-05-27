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

#include "../../../direct/direct_provider.hh"
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

    // Provider has failed sanity check, thus we can't provide Direct
    if (shiro::direct::provider == nullptr) {
        response.code = 502;
        response.end();
        return;
    }

    auto [success, output] = shiro::direct::provider->download(id, no_video);

    if (!success) {
        response.code = 504;
        response.end();
        return;
    }

    response.set_header("Content-Type", "application/octet-stream; charset=UTF-8");
    response.end(output);
}
