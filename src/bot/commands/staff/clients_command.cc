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
#include "clients_command.hh"
#include "../../../utils/bot_utils.hh"

std::tuple<std::string, std::string> to_string(const shiro::utils::osu_client &client) {
    switch (client) {
        case shiro::utils::osu_client::unknown:
            return { "[!]", "Unknown" };
        case shiro::utils::osu_client::stable:
            return { "", "osu!Stable" };
        case shiro::utils::osu_client::beta:
            return { "", "osu!Beta" };
        case shiro::utils::osu_client::cutting_edge:
            return { "", "osu!cutting edge" };
        case shiro::utils::osu_client::fallback:
            return { "", "osu!fallback" };
        case shiro::utils::osu_client::dev:
            return { "[!]", "osu!dev" };
        case shiro::utils::osu_client::public_test:
            return { "[!]", "osu!public testing" };
        case shiro::utils::osu_client::no_xna:
            return { "[!]", "osu!no xna" };
        case shiro::utils::osu_client::lazer:
            return { "", "osu!lazer" };
        case shiro::utils::osu_client::tournament:
            return { "", "osu!tournament" };
        case shiro::utils::osu_client::osu_fx:
            return { "[3rd]", "osu!fx" };
        case shiro::utils::osu_client::banana_client:
            return { "[3rd]", "Banana client" };
        case shiro::utils::osu_client::tsuki:
            return { "[3rd]", "tsuki" };
    }

    return { "[!]", "Unknown" };
}

bool shiro::bot::commands::clients(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    utils::bot::respond("List of online users and their client:", user, channel, true);
    utils::bot::respond("[Prefix] Client - Username (Sent version -> Parsed version)", user, channel, true);

    for (const std::shared_ptr<users::user> &online_user : users::manager::online_users) {
        auto [prefix, client] = to_string(user->client_type);

        if (online_user->user_id == 1) {
            prefix = "[S]";
            client = "Aschente";
        }

        std::string begin = prefix.empty() ? client : prefix.append(" ").append(client);
        std::string msg = begin + " - " + online_user->presence.username + " (" + online_user->client_version + " -> " + std::to_string(online_user->client_build) + ")";

        utils::bot::respond(msg, user, channel, true);
    }

    return true;
}
