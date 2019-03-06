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

#include "../../permissions/role_manager.hh"
#include "../../users/user_manager.hh"
#include "../../utils/bot_utils.hh"
#include "announce_command.hh"

bool shiro::commands::announce(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    if (args.empty()) {
        utils::bot::respond("Usage: !announce [user] <announcement>", user, channel, true);
        return false;
    }

    if (!roles::manager::has_permission(user, permissions::perms::cmd_announce)) {
        utils::bot::respond("Permission denied. (" + std::to_string((uint64_t) permissions::perms::cmd_announce) + ")", user, channel, true);
        return false;
    }

    std::shared_ptr<users::user> target = nullptr;

    if (args.size() >= 2) {
        target = users::manager::get_user_by_username(args.at(0));

        if (target != nullptr && users::manager::is_online(target))
            args.pop_front();
    }

    std::string announcement;
    for (const std::string &arg : args) {
        announcement += " " + arg;
    }

    io::osu_writer writer;
    writer.announce(announcement);

    if (target != nullptr) {
        target->queue.enqueue(writer);
        utils::bot::respond("Successfully sent a announcement to " + target->presence.username + ".", user, channel, true);
        return true;
    }

    users::manager::iterate([&writer](std::shared_ptr<users::user> online_user) {
        online_user->queue.enqueue(writer);
    }, true);

    utils::bot::respond("Successfully sent a announcement to everyone.", user, channel, true);
    return true;
}
