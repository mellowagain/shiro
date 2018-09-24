#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cmath>

#include "../channels/channel_manager.hh"
#include "../geoloc/geoloc.hh"
#include "../geoloc/location_info.hh"
#include "../io/layouts/packets.hh"
#include "../io/osu_writer.hh"
#include "../thirdparty/digestpp.hh"
#include "../thirdparty/loguru.hh"
#include "../thirdparty/uuid.hh"
#include "../users/user.hh"
#include "../users/user_manager.hh"
#include "../users/user_punishments.hh"
#include "../utils/bot_utils.hh"
#include "../utils/login_responses.hh"
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

        LOG_F(WARNING, "Received invalid login request from %s: Login body has wrong length.", request.get_header_value("X-Forwarded-For").c_str());
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

        LOG_F(WARNING, "Received invalid login request from %s: Additional info has wrong length.", request.get_header_value("X-Forwarded-For").c_str());
        return;
    }

    io::osu_writer writer;
    writer.protocol_negotiation(shiro::io::cho_protocol);

    std::shared_ptr<users::user> user = std::make_shared<users::user>(username);

    if (!user->init()) {
        writer.login_reply((int32_t) utils::login_responses::invalid_credentials);

        response.end(writer.serialize());

        LOG_F(WARNING, "%s (%s) tried to login as non-existent user.", username.c_str(), request.get_header_value("X-Forwarded-For").c_str());
        return;
    }

    if (!user->check_password(password_md5)) {
        writer.login_reply((int32_t) utils::login_responses::invalid_credentials);

        response.end(writer.serialize());

        LOG_F(WARNING, "%s (%s) tried to login with wrong password.", username.c_str(), request.get_header_value("X-Forwarded-For").c_str());
        return;
    }

    if (users::punishments::is_banned(user->user_id)) {
        writer.login_reply((int32_t) utils::login_responses::user_banned);

        response.end(writer.serialize());

        LOG_F(WARNING, "%s (%s) tried to login while being banned.", username.c_str(), request.get_header_value("X-Forwarded-For").c_str());
        return;
    }

    std::string version = additional_info.at(0);
    std::string utc_offset = additional_info.at(1);
    std::string hwid = additional_info.at(3);
    int32_t build = 20131216;

    try {
        build = boost::lexical_cast<int32_t>(version.substr(1, version.find('.') - 1));
    } catch (const boost::bad_lexical_cast &ex) {
        LOG_S(WARNING) << "Unable to cast " << version << " to int64_t: " << ex.what();
    } catch (const std::out_of_range &ex) {
        LOG_S(WARNING) << "Unable to convert client version " << version << " is valid build: " << ex.what();
    }

    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    user->token = sole::uuid4().str();
    user->client_version = version;
    user->client_build = build;
    user->hwid = digestpp::sha256().absorb(hwid).hexdigest();
    user->last_ping = seconds;

    uint8_t time_zone = 9;

    try {
        time_zone = boost::lexical_cast<uint8_t>(utc_offset);
    } catch (const boost::bad_lexical_cast &ex) {
        LOG_S(WARNING) << "Unable to cast " << utc_offset << " to uint8_t: " << ex.what() << ".";
    }

    geoloc::location_info info = geoloc::get_location(request.get_header_value("X-Forwarded-For"));
    user->presence.country_id = info.country;
    user->presence.time_zone = time_zone;
    user->presence.latitude = info.latitude;
    user->presence.longitude = info.longitude;

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

    for (const std::shared_ptr<users::user> &online_user : users::manager::online_users) {
        if (online_user == user)
            continue;

        writer.user_presence(online_user->presence);

        if (!user->hidden)
            online_user->queue.enqueue(global_writer);
    }

    std::string result = writer.serialize();

    if (!user->queue.is_empty())
        result.append(user->queue.serialize());

    response.end(result);
}
