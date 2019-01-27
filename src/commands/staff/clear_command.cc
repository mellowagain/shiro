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

#include "../../config/bot_file.hh"
#include "../../permissions/role_manager.hh"
#include "../../users/user_manager.hh"
#include "../../utils/bot_utils.hh"
#include "clear_command.hh"

bool shiro::commands::clear(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    std::shared_ptr<users::user> target = nullptr;

    if (args.size() >= 2) {
        utils::bot::respond("Usage: !clear [user]", user, channel, true);
        return false;
    }

    if (!roles::manager::has_permission(user, permissions::perms::cmd_clear)) {
        utils::bot::respond("Permission denied. (" + std::to_string((uint64_t) permissions::perms::cmd_clear) + ")", user, channel, true);
        return false;
    }

    if (args.size() == 1) {
        target = users::manager::get_user_by_username(args.at(0));

        if (target == nullptr) {
            utils::bot::respond(args.at(0) + " is currently not online or does not exist.", user, channel, true);
            return false;
        }
    }

    io::osu_writer writer;

    for (const std::shared_ptr<users::user> &online_user : users::manager::online_users) {
        writer.user_silenced(online_user->user_id);
    }

    if (target != nullptr) {
        target->queue.enqueue(writer);
        utils::bot::respond("Successfully cleared chat for " + target->presence.username + ".", user, channel, true);
        return true;
    }

    for (const std::shared_ptr<users::user> &online_user : users::manager::online_users) {
        if (online_user->user_id == 1)
            continue;

        online_user->queue.enqueue(writer);
        utils::bot::respond("Your chat was cleared by " + user->presence.username + ".", online_user, config::bot::name, true);
    }

    utils::bot::respond("Successfully cleared chat for all online users.", user, channel, true);
    return true;
}
