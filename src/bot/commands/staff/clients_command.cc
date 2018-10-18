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

#include "../../../users/user_manager.hh"
#include "../../../utils/bot_utils.hh"
#include "../../../utils/osu_client.hh"
#include "clients_command.hh"

bool shiro::bot::commands::clients(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    if (args.size() >= 2) {
        utils::bot::respond("Usage: !clients [user]", user, channel, true);
        return false;
    }

    if (args.size() == 1) {
        std::shared_ptr<users::user> target = users::manager::get_user_by_username(args.at(0));

        if (target == nullptr) {
            utils::bot::respond(args.at(1) + " is currently not online or does not exist.", user, channel, true);
            return false;
        }

        char buffer[256];
        std::snprintf(
                buffer, sizeof(buffer),
                "%s is using %s. (%s -> %i)",
                target->presence.username.c_str(),
                utils::clients::to_pretty_string(utils::clients::osu_client::_from_integral(target->client_type)).c_str(),
                target->client_version.c_str(),
                target->client_build
        );

        utils::bot::respond(buffer, user, channel, true);
        return true;
    }

    std::vector<std::string> lines;

    for (const utils::clients::osu_client &client : utils::clients::osu_client::_values()) {
        std::vector<std::shared_ptr<users::user>> users = utils::clients::get_users(client);

        if (users.empty()) {
            lines.emplace_back(utils::clients::to_pretty_string(client) + " (0):");
            continue;
        }

        std::string line = utils::clients::to_pretty_string(client) + " (" + std::to_string(users.size()) + "): ";

        for (const std::shared_ptr<users::user> &client_user : users) {
            line += client_user->presence.username + ", ";
        }

        lines.emplace_back(line.substr(0, line.size() - 2));
    }

    for (const std::string &line : lines) {
        utils::bot::respond(line, user, channel, true);
    }

    return true;
}
