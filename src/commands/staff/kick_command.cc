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

#include <sstream>

#include "../../permissions/permissions.hh"
#include "../../permissions/role_manager.hh"
#include "../../users/user_manager.hh"
#include "../../users/user_punishments.hh"
#include "../../utils/bot_utils.hh"
#include "kick_command.hh"

bool shiro::commands::kick(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    if (args.size() < 2) {
        utils::bot::respond("Usage: !kick <user> [reason]", user, channel, true);
        return false;
    }

    if (!roles::manager::has_permission(user, permissions::perms::cmd_kick)) {
        utils::bot::respond("Permission denied. (" + std::to_string((uint64_t) permissions::perms::cmd_kick) + ")", user, channel, true);
        return false;
    }

    std::shared_ptr<users::user> target = users::manager::get_user_by_username(args.at(0));
    std::string reason = "You have been kicked.";

    if (target->user_id == 1) {
        utils::bot::respond("Don\'t try to kick robots, silly human!", user, channel, true);
        return false;
    }

    if (args.size() >= 3) {
        std::stringstream stream;

        args.pop_front();

        for (const std::string &arg : args) {
            stream << arg << " ";
        }

        reason = stream.str();
        reason.pop_back();
    }

    users::punishments::kick(target->user_id, user->user_id, reason);
    utils::bot::respond(target->presence.username + " has been kicked for " + reason + ".", user, channel, true);
    return true;
}
