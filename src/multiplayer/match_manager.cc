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

#include "match_manager.hh"
#include "../utils/slot_status.hh"
#include "../utils/match_team_type.hh"
#include "../users/user_manager.hh"
#include "lobby_manager.hh"

std::vector<shiro::io::layouts::multiplayer_match> shiro::multiplayer::match_manager::matches;
std::shared_timed_mutex shiro::multiplayer::match_manager::mutex;

uint16_t shiro::multiplayer::match_manager::highest_match_id = 0;

void shiro::multiplayer::match_manager::create_match(shiro::io::layouts::multiplayer_match &match) {
    if (match.game_name.length() > 50)
        match.game_name.resize(50);

    // Disallow other threads from both writing and reading
    std::unique_lock<std::shared_timed_mutex> lock(mutex);

    match.match_id = highest_match_id;
    highest_match_id++;

    matches.emplace_back(match);

    // We're done with actions that can't be done in parallel, let's unlock
    lock.unlock();

    io::osu_writer writer;
    writer.match_new(match);

    // Store the password in a temporary variable
    std::string backup = match.game_password;

    // Set the password to non-sense to prevent eavesdropping
    match.game_password = "turn my swag on";

    io::osu_writer global_writer;
    global_writer.match_new(match);

    // Restore it as the match is a reference
    match.game_password = backup;

    lobby_manager::iterate([match, &writer, &global_writer](std::shared_ptr<users::user> user) {
        if (user->user_id == match.host_id) {
            user->queue.enqueue(writer);
            return;
        }

        user->queue.enqueue(global_writer);
    });
}

bool shiro::multiplayer::match_manager::join_match(io::layouts::multiplayer_join request, std::shared_ptr<shiro::users::user> user) {
    if (user == nullptr || user->hidden)
        return false;

    if (in_match(user))
        leave_match(user);

    // Disallow other threads from both writing and reading
    std::unique_lock<std::shared_timed_mutex> lock(mutex);

    for (io::layouts::multiplayer_match &match : matches) {
        if (match.match_id != request.match_id)
            continue;

        if (!match.game_password.empty() && match.game_password != request.password)
            return false;

        // Find the next open slot (if available)
        auto iterator = std::find(match.multi_slot_id.begin(), match.multi_slot_id.end(), 0);

        if (iterator == match.multi_slot_id.end())
            return false;

        uint64_t index = (uint64_t) std::distance(match.multi_slot_id.begin(), iterator);
        uint8_t team = (uint8_t) (utils::is_team(match.multi_team_type) ? index % 2 + 1 : 0);

        match.multi_slot_id.at(index) = user->user_id;
        match.multi_slot_status.at(index) = (uint8_t) utils::slot_status::not_ready;
        match.multi_slot_team.at(index) = team;
        match.multi_slot_mods.at(index) = 0;

        return true;
    }

    return false;
}

bool shiro::multiplayer::match_manager::leave_match(std::shared_ptr<shiro::users::user> user) {
    if (user == nullptr || user->hidden || !in_match(user))
        return false;

    // Disallow other threads from both writing and reading
    std::unique_lock<std::shared_timed_mutex> lock(mutex);
    int16_t match_id = -1;

    for (io::layouts::multiplayer_match &match : matches) {
        auto iterator = std::find(match.multi_slot_id.begin(), match.multi_slot_id.end(), user->user_id);

        if (iterator == match.multi_slot_id.end())
            continue;

        uint64_t index = (uint64_t) std::distance(match.multi_slot_id.begin(), iterator);

        match_id = match.match_id;

        match.multi_slot_id.at(index) = 0;
        match.multi_slot_status.at(index) = (uint8_t) utils::slot_status::open;
        match.multi_slot_team.at(index) = 0;
        match.multi_slot_mods.at(index) = 0;
    }

    if (match_id == -1)
        return false;

    auto iterator = std::find_if(matches.begin(), matches.end(), [match_id](const io::layouts::multiplayer_match &match) {
        return match.match_id == match_id;
    });

    // How does this even happen?
    if (iterator == matches.end())
        return false;

    io::layouts::multiplayer_match match = *iterator;

    auto first_non_empty = std::find_if_not(match.multi_slot_id.begin(), match.multi_slot_id.end(), [](int32_t id) {
        return id == 0;
    });

    // The match is empty, destroy it
    if (first_non_empty == match.multi_slot_id.end())
        matches.erase(iterator);

    return true;
}

bool shiro::multiplayer::match_manager::in_match(std::shared_ptr<shiro::users::user> user) {
    // Disallow other threads from writing (but not from reading)
    std::shared_lock<std::shared_timed_mutex> lock(mutex);

    for (const io::layouts::multiplayer_match &match : matches) {
        auto iterator = std::find(match.multi_slot_id.begin(), match.multi_slot_id.end(), user->user_id);

        if (iterator == match.multi_slot_id.end())
            continue;

        return true;
    }

    return false;
}

std::optional<shiro::io::layouts::multiplayer_match> shiro::multiplayer::match_manager::get_match(uint16_t match_id) {
    // Disallow other threads from writing (but not from reading)
    std::shared_lock<std::shared_timed_mutex> lock(mutex);

    for (const io::layouts::multiplayer_match &match : matches) {
        if (match.match_id != match_id)
            continue;

        return match;
    }

    return std::nullopt;
}

std::optional<shiro::io::layouts::multiplayer_match> shiro::multiplayer::match_manager::get_match(std::shared_ptr<shiro::users::user> user) {
    // Disallow other threads from writing (but not from reading)
    std::shared_lock<std::shared_timed_mutex> lock(mutex);

    for (const io::layouts::multiplayer_match &match : matches) {
        auto iterator = std::find(match.multi_slot_id.begin(), match.multi_slot_id.end(), user->user_id);

        if (iterator == match.multi_slot_id.end())
            continue;

        return match;
    }

    return std::nullopt;
}

void shiro::multiplayer::match_manager::iterate(const std::function<void(io::layouts::multiplayer_match)> &callback) {
    // Disallow other threads from writing (but not from reading)
    std::shared_lock<std::shared_timed_mutex> lock(mutex);

    for (const io::layouts::multiplayer_match &match : matches) {
        callback(match);
    }
}
