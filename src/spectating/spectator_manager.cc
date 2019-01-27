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

#include "spectator_manager.hh"

std::vector<std::pair<std::shared_ptr<shiro::users::user>, std::shared_ptr<shiro::users::user>>> shiro::spectating::manager::currently_spectating;

void shiro::spectating::manager::start_spectating(std::shared_ptr<shiro::users::user> user, std::shared_ptr<shiro::users::user> target) {
    if (is_spectating(user))
        stop_spectating(user);

    io::osu_writer writer;
    writer.channel_join("#spectator");

    user->queue.enqueue(writer);

    if (get_spectators(target).empty())
        target->queue.enqueue(writer);

    currently_spectating.emplace_back(std::make_pair(user, target));
}

void shiro::spectating::manager::stop_spectating(std::shared_ptr<shiro::users::user> user) {
    if (!is_spectating(user))
        return;

    for (size_t i = 0; i < currently_spectating.size(); i++) {
        const auto &[host, _] = currently_spectating.at(i);

        if (host == user) {
            currently_spectating.erase(currently_spectating.begin() + i);

            io::osu_writer writer;
            writer.channel_revoked("#spectator");

            user->queue.enqueue(writer);

            if (get_spectators(host).empty())
                host->queue.enqueue(writer);

            break;
        }
    }
}

bool shiro::spectating::manager::is_spectating(std::shared_ptr<shiro::users::user> user) {
    for (const auto &[host, _] : currently_spectating) {
        if (host == user)
            return true;
    }

    return false;
}

bool shiro::spectating::manager::is_being_spectated(std::shared_ptr<shiro::users::user> user) {
    for (const auto &[_, spectator] : currently_spectating) {
        if (spectator == user)
            return true;
    }

    return false;
}

std::vector<std::shared_ptr<shiro::users::user>> shiro::spectating::manager::get_spectators(std::shared_ptr<shiro::users::user> user) {
    std::vector<std::shared_ptr<shiro::users::user>> users;

    for (const auto &[host, spectator] : currently_spectating) {
        if (spectator == user)
            users.emplace_back(host);
    }

    return users;
}

std::shared_ptr<shiro::users::user> shiro::spectating::manager::get_host(std::shared_ptr<shiro::users::user> user) {
    for (const auto &[host, spectator] : currently_spectating) {
        if (host == user)
            return spectator;
    }

    return nullptr;
}
