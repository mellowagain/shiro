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

#include "../../../config/score_submission_file.hh"
#include "../../../logger/sentry_logger.hh"
#include "../../../thirdparty/loguru.hh"
#include "../../../users/user.hh"
#include "../../../users/user_punishments.hh"
#include "../../../utils/client_side_flags.hh"
#include "lastfm_route.hh"

void shiro::routes::web::lastfm::handle(const crow::request &request, crow::response &response) {
    response.set_header("Content-Type", "text/plain; charset=UTF-8");
    response.set_header("cho-server", "shiro (https://github.com/Marc3842h/shiro)");

    char *beatmap = request.url_params.get("b");
    char *username = request.url_params.get("us");
    char *md5sum = request.url_params.get("ha");

    if (beatmap == nullptr || username == nullptr || md5sum == nullptr) {
        response.code = 400;
        response.end();
        return;
    }

    std::shared_ptr<users::user> user = std::make_shared<users::user>(username);

    if (!user->init()) {
        response.code = 400;
        response.end();
        return;
    }

    if (!user->check_password(md5sum)) {
        response.code = 401;
        response.end();
        return;
    }

    std::string beatmap_str = beatmap;

    // Stop processing normal requests
    if (!boost::algorithm::starts_with(beatmap_str, "a")) {
        response.code = 200;
        response.end();
        return;
    }

    int32_t startup_value = 0;

    try {
        startup_value = boost::lexical_cast<int32_t>(beatmap_str.substr(1));
    } catch (const boost::bad_lexical_cast &ex) {
        LOG_F(WARNING, "Unable to cast %s to int32_t: %s", beatmap_str.substr(1).c_str(), ex.what());
        logging::sentry::exception(ex);

        response.code = 500;
        response.end();
        return;
    }

    for (utils::client_side_flags flag : utils::client_side_flags::_values()) {
        int32_t numeric_flag = flag;

        if (!(startup_value & numeric_flag))
            continue;

        std::string result = flag._to_string();
        std::replace(result.begin(), result.end(), '_', ' ');
        result.at(0) = std::toupper(result.at(0));

        std::string::size_type index = result.find(' ');

        if (index != std::string::npos)
            result.at(index + 1) = std::toupper(result.at(index + 1));

        LOG_F(WARNING, "%s has client flag set: %s (%i & %i).", username, result.c_str(), startup_value, numeric_flag);

        if (!config::score_submission::consider_client_side_flags || numeric_flag < 8)
            continue;

        users::punishments::restrict(user->user_id, 10, "Detected client flag: " + result);
    }
}
