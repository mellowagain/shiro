/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2020 Marc3842h, czapek
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

#include "lobby_manager.hh"

std::vector<std::shared_ptr<shiro::users::user>> shiro::multiplayer::lobby_manager::users;
std::shared_timed_mutex shiro::multiplayer::lobby_manager::mutex;

void shiro::multiplayer::lobby_manager::add_user(std::shared_ptr<shiro::users::user> user) {
    if (user == nullptr || in_lobby(user))
        return;

    // Disallow other threads from both writing and reading
    std::unique_lock<std::shared_timed_mutex> lock(mutex);

    users.emplace_back(user);
}

void shiro::multiplayer::lobby_manager::remove_user(std::shared_ptr<shiro::users::user> user) {
    if (user == nullptr || !in_lobby(user))
        return;

    // Disallow other threads from both writing and reading
    std::unique_lock<std::shared_timed_mutex> lock(mutex);

    auto iterator = std::find(users.begin(), users.end(), user);

    if (iterator == users.end())
        return;

    users.erase(iterator);
}

bool shiro::multiplayer::lobby_manager::in_lobby(std::shared_ptr<shiro::users::user> user) {
    if (user == nullptr)
        return false;

    // Disallow other threads from writing (but not from reading)
    std::shared_lock<std::shared_timed_mutex> lock(mutex);

    return std::find(users.begin(), users.end(), user) != users.end();
}

void shiro::multiplayer::lobby_manager::iterate(const std::function<void(std::shared_ptr<shiro::users::user>)> &callback) {
    // Disallow other threads from writing (but not from reading)
    std::shared_lock<std::shared_timed_mutex> lock(mutex);

    for (const std::shared_ptr<users::user> &user : users) {
        callback(user);
    }
}
