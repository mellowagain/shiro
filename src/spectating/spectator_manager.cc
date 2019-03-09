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

// spectating user - hoster (sends data)
std::vector<std::pair<std::shared_ptr<shiro::users::user>, std::shared_ptr<shiro::users::user>>> shiro::spectating::manager::currently_spectating;
std::shared_timed_mutex shiro::spectating::manager::mutex;

void shiro::spectating::manager::start_spectating(std::shared_ptr<shiro::users::user> user, std::shared_ptr<shiro::users::user> target) {
    if (is_spectating(user))
        stop_spectating(user);

    io::osu_writer writer;
    writer.channel_join("#spectator");

    user->queue.enqueue(writer);

    if (get_spectators(target).empty())
        target->queue.enqueue(writer);

    // Disallow other threads from both writing and reading
    std::unique_lock<std::shared_timed_mutex> lock(mutex);

    currently_spectating.emplace_back(std::make_pair(user, target));
}

void shiro::spectating::manager::stop_spectating(std::shared_ptr<shiro::users::user> user) {
    if (!is_spectating(user))
        return;

    io::osu_writer writer;
    writer.channel_revoked("#spectator");

    auto callback = [user, &writer](std::pair<std::shared_ptr<users::user>, std::shared_ptr<users::user>> pair) -> bool {
        const auto &[spectator, host] = pair;
        bool lonely = true; // 2meirl4meirl

        if (spectator != user)
            return false;

        user->queue.enqueue(writer);

        // Check if the host is now lonely by iterating over the whole list again (goodbye performance)
        // I would use is_spectating here but we would dead lock if we call it from here
        for (const auto &[_, iterated_host] : currently_spectating) {
            if (iterated_host != host)
                continue;

            lonely = false;
            break;
        }

        if (lonely)
            host->queue.enqueue(writer);

        return true;
    };

    // Disallow other threads from both writing and reading
    std::unique_lock<std::shared_timed_mutex> lock(mutex);

    currently_spectating.erase(
            std::remove_if(currently_spectating.begin(), currently_spectating.end(), callback), currently_spectating.end()
    );
}

bool shiro::spectating::manager::is_spectating(std::shared_ptr<shiro::users::user> user) {
    // Disallow other threads from writing (but not from reading)
    std::shared_lock<std::shared_timed_mutex> lock(mutex);

    for (const auto &[spectator, _] : currently_spectating) {
        if (spectator != user)
            continue;

        return true;
    }

    return false;
}

bool shiro::spectating::manager::is_being_spectated(std::shared_ptr<shiro::users::user> user) {
    // Disallow other threads from writing (but not from reading)
    std::shared_lock<std::shared_timed_mutex> lock(mutex);

    for (const auto &[_, host] : currently_spectating) {
        if (host != user)
            continue;

        return true;
    }

    return false;
}

std::vector<std::shared_ptr<shiro::users::user>> shiro::spectating::manager::get_spectators(std::shared_ptr<shiro::users::user> user) {
    std::vector<std::shared_ptr<shiro::users::user>> users;
    std::shared_lock<std::shared_timed_mutex> lock(mutex);

    for (const auto &[spectator, host] : currently_spectating) {
        if (host != user)
            continue;

        users.emplace_back(spectator);
    }

    return users;
}

std::shared_ptr<shiro::users::user> shiro::spectating::manager::get_host(std::shared_ptr<shiro::users::user> user) {
    // Disallow other threads from writing (but not from reading)
    std::shared_lock<std::shared_timed_mutex> lock(mutex);

    for (const auto &[spectator, host] : currently_spectating) {
        if (spectator != user)
            continue;

        return host;
    }

    return nullptr;
}
