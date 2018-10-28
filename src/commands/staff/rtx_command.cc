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

#include "../../users/user_manager.hh"
#include "../../utils/bot_utils.hh"
#include "rtx_command.hh"
#include "../../permissions/role_manager.hh"

bool shiro::commands::rtx(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    if (args.empty()) {
        utils::bot::respond("Usage: !rtx <target> [reason]", user, channel, true);
        return false;
    }

    if (!roles::manager::has_permission(user, permissions::perms::cmd_rtx)) {
        utils::bot::respond("Permission denied. (" + std::to_string((uint64_t) permissions::perms::cmd_rtx) + ")", user, channel, true);
        return false;
    }

    std::shared_ptr<users::user> target = users::manager::get_user_by_username(args.at(0));

    if (target == nullptr || !users::manager::is_online(target)) {
        utils::bot::respond(args.at(0) + " is currently not online or does not exist.", user, channel, true);
        return false;
    }

    // No reason has been provided, set default reason
    if (args.size() == 1)
        args.emplace_back("rtx'ed by " + user->presence.username);

    args.pop_front();

    std::string reason;
    for (const std::string &arg : args) {
        reason += " " + arg;
    }

    io::osu_writer writer;
    writer.rtx(reason);

    target->queue.enqueue(writer);
    utils::bot::respond(target->presence.username + " has been successfully rtx'ed.", user, channel, true);
    return true;
}
