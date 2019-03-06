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

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cmath>

#include "../channels/channel_manager.hh"
#include "../config/score_submission_file.hh"
#include "../geoloc/geoloc.hh"
#include "../geoloc/location_info.hh"
#include "../io/layouts/packets.hh"
#include "../io/osu_writer.hh"
#include "../logger/sentry_logger.hh"
#include "../thirdparty/digestpp.hh"
#include "../thirdparty/loguru.hh"
#include "../thirdparty/uuid.hh"
#include "../users/user.hh"
#include "../users/user_manager.hh"
#include "../users/user_punishments.hh"
#include "../utils/bot_utils.hh"
#include "../utils/login_responses.hh"
#include "../utils/osu_client.hh"
#include "../utils/string_utils.hh"
#include "login_handler.hh"

void shiro::handler::login::handle(const crow::request &request, crow::response &response) {
    if (request.body.empty()) {
        response.end();
        return;
    }

    std::vector<std::string> lines;
    boost::split(lines, request.body, boost::is_any_of("\n"));

    if (lines.size() != 4) {
        response.code = 403;
        response.end();

        LOG_F(WARNING, "Received invalid login request from %s: Login body has wrong length (%lu != 4).", request.get_ip_address().c_str(), lines.size());
        return;
    }

    std::string username = lines.at(0);
    std::string password_md5 = lines.at(1);
    std::string infos = lines.at(2);

    std::vector<std::string> additional_info;
    boost::split(additional_info, infos, boost::is_any_of("|"));

    if (additional_info.size() != 5) {
        response.code = 403;
        response.end();

        LOG_F(WARNING, "Received invalid login request from %s: Additional info has wrong length.", request.get_ip_address().c_str());
        return;
    }

    io::osu_writer writer;
    writer.protocol_negotiation(io::cho_protocol);

    std::shared_ptr<users::user> user = std::make_shared<users::user>(username);

    if (!user->init()) {
        writer.login_reply((int32_t) utils::login_responses::invalid_credentials);

        response.end(writer.serialize());

        LOG_F(WARNING, "%s (%s) tried to login as non-existent user.", username.c_str(), request.get_ip_address().c_str());
        return;
    }

    if (!user->check_password(password_md5)) {
        writer.login_reply((int32_t) utils::login_responses::invalid_credentials);

        response.end(writer.serialize());

        LOG_F(WARNING, "%s (%s) tried to login with wrong password.", username.c_str(), request.get_ip_address().c_str());
        return;
    }

    if (users::punishments::is_banned(user->user_id)) {
        writer.login_reply((int32_t) utils::login_responses::user_banned);

        response.end(writer.serialize());

        LOG_F(WARNING, "%s (%s) tried to login while being banned.", username.c_str(), request.get_ip_address().c_str());
        return;
    }

    std::string version = additional_info.at(0);
    std::string utc_offset = additional_info.at(1);
    std::string hwid = additional_info.at(3);
    int32_t build = 20131216;

    std::string parseable_version;
    std::string::size_type dot_index = version.find('.');

    if (dot_index != std::string::npos) {
        parseable_version = version.substr(1, dot_index - 1);
    } else {
        parseable_version = version;

        parseable_version.erase(std::remove_if(parseable_version.begin(), parseable_version.end(), [](char c) {
            return !std::isdigit(c);
        }), parseable_version.end());
    }

    try {
        build = boost::lexical_cast<int32_t>(parseable_version);
    } catch (const boost::bad_lexical_cast &ex) {
        LOG_F(WARNING, "Unable to cast %s to int32_t: %s", version.c_str(), ex.what());
        logging::sentry::exception(ex);

        if (config::score_submission::restrict_mismatching_client_version) {
            writer.login_reply((int32_t) utils::login_responses::server_error);

            response.end(writer.serialize());
            return;
        }
    } catch (const std::out_of_range &ex) {
        LOG_F(WARNING, "Unable to convert client version %s to valid build: %s.", version.c_str(), ex.what());
        logging::sentry::exception(ex);

        if (config::score_submission::restrict_mismatching_client_version) {
            writer.login_reply((int32_t) utils::login_responses::server_error);

            response.end(writer.serialize());
            return;
        }
    }

    user->client_type = utils::clients::parse_version(version, build);

    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    user->token = sole::uuid4().str();
    user->client_version = version;
    user->client_build = build;
    user->hwid = digestpp::sha256().absorb(hwid).hexdigest();
    user->last_ping = seconds;

    int8_t time_zone = 9;

    try {
        time_zone = (int8_t) boost::lexical_cast<int32_t>(utc_offset);
    } catch (const boost::bad_lexical_cast &ex) {
        LOG_F(WARNING, "Unable to cast %s to int32_t (int8_t): %s.", utc_offset.c_str(), ex.what());
        logging::sentry::exception(ex);
    }

    std::optional<geoloc::location_info> info = geoloc::get_location(request.get_ip_address());

    if (info.has_value()) {
        geoloc::location_info location_info = info.value();

        user->presence.country_id = location_info.country;
        user->presence.latitude = location_info.latitude;
        user->presence.longitude = location_info.longitude;
    }

    user->presence.time_zone = time_zone;

    users::manager::login_user(user);

    response.set_header("cho-token", user->token);

    writer.login_reply(user->user_id);
    writer.login_permissions(user->presence.permissions);
    writer.announce("Welcome to shiro!");

    writer.friend_list(user->friends);

    writer.channel_listing_complete();
    channels::manager::write_channels(writer, user);

    io::osu_writer global_writer;
    global_writer.user_presence(user->presence);

    if (users::punishments::is_silenced(user->user_id)) {
        auto [timestamp, duration] = users::punishments::get_silence_time(user->user_id);

        writer.user_ban_info((timestamp + duration) - seconds.count());
        global_writer.user_silenced(user->user_id);
    }

    if (users::punishments::is_restricted(user->user_id)) {
        utils::bot::respond(
                "[https://shiro.host/u/me Your account has been restricted]. "
                "Because of that, your profile has been hidden from the public. "
                "If you believe this is a mistake, [https://shiro.host/support contact support] "
                "to have your account status reviewed.",
                user, config::bot::name, true
        );
    }

    users::manager::iterate([user, &writer, &global_writer](std::shared_ptr<users::user> online_user) {
        if (online_user == user)
            return;

        writer.user_presence(online_user->presence);

        if (!user->hidden)
            online_user->queue.enqueue(global_writer);
    }, true);

    std::string result = writer.serialize();

    if (!user->queue.is_empty())
        result.append(user->queue.serialize());

    response.end(result);
}
