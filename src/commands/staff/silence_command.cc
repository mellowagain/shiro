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

#include <boost/lexical_cast.hpp>

#include "../../permissions/role_manager.hh"
#include "../../users/user_manager.hh"
#include "../../users/user_punishments.hh"
#include "../../utils/bot_utils.hh"
#include "silence_command.hh"

#include "../../thirdparty/loguru.hh"
bool shiro::commands::silence(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    if (args.size() < 3) {
        utils::bot::respond("Usage: !silence <user> <duration>[s,min,h,d,w,m] <reason>", user, channel, true);
        return false;
    }

    if (!roles::manager::has_permission(user, permissions::perms::cmd_silence)) {
        utils::bot::respond("Permission denied. (" + std::to_string((uint64_t) permissions::perms::cmd_silence) + ")", user, channel, true);
        return false;
    }

    int32_t target = users::manager::get_id_by_username(args.at(0));
    std::string target_username = args.at(0);
    std::stringstream stream;

    if (target == -1) {
        utils::bot::respond(target_username + " could not be found.", user, channel, true);
        return false;
    }

    if (target == 1) {
        utils::bot::respond("Don\'t try to silence robots, silly human!", user, channel, true);
        return false;
    }

    std::string raw_time = args.at(1);
    std::string time_modifier = "s";
    uint32_t time = 0;

    if (raw_time.find("min") != std::string::npos) {
        time_modifier = raw_time.substr(raw_time.length() - 3);
        raw_time = raw_time.substr(0, raw_time.length() - 3);
    } else {
        time_modifier = raw_time.back();
        raw_time = raw_time.substr(0, raw_time.length() - 1);
    }

    try {
        LOG_F(INFO, "Converting %s to uint.", raw_time.c_str());
        time = boost::lexical_cast<uint32_t>(raw_time);
    } catch (const boost::bad_lexical_cast &ex) {
        utils::bot::respond("Unable to convert duration into valid unsigned integer.", user, channel, true);
        return false;
    }

    if (time_modifier == "min") {
        time *= 60;
    } else if (time_modifier == "h") {
        time *= 60 * 60;
    } else if (time_modifier == "d") {
        time *= 60 * 60 * 24;
    } else if (time_modifier == "w") {
        time *= 60 * 60 * 24 * 7;
    } else if (time_modifier == "m") {
        time *= 60 * 60 * 24 * 7 * 30;
    }

    // Remove username and duration
    args.pop_front();
    args.pop_front();

    for (const std::string &arg : args) {
        stream << arg << " ";
    }

    std::string reason = stream.str();
    reason.pop_back();

    users::punishments::silence(target, user->user_id, time, reason);
    utils::bot::respond(target_username + " has been silenced for " + std::to_string(time) + " seconds.", user, channel, true);
    return true;
}
