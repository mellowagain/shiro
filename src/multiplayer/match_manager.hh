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

#ifndef SHIRO_MATCH_MANAGER_HH
#define SHIRO_MATCH_MANAGER_HH

#include <optional>
#include <shared_mutex>
#include <vector>

#include "../io/layouts/multiplayer/multiplayer_join.hh"
#include "../io/layouts/multiplayer/multiplayer_match.hh"
#include "../users/user.hh"

namespace shiro::multiplayer::match_manager {

    extern std::vector<io::layouts::multiplayer_match> matches;
    extern std::shared_timed_mutex mutex;

    extern uint16_t highest_match_id;

    void create_match(io::layouts::multiplayer_match &match);

    std::optional<io::layouts::multiplayer_match> join_match(io::layouts::multiplayer_join request, std::shared_ptr<users::user> user);

    // This also additionally destroys the match if no users are remaining in the room
    bool leave_match(std::shared_ptr<users::user> user);

    bool in_match(std::shared_ptr<users::user> user);

    std::optional<io::layouts::multiplayer_match> get_match(uint16_t match_id);
    std::optional<io::layouts::multiplayer_match> get_match(std::shared_ptr<users::user> user);

    // This iterate function takes a reference and thus allows persistent modification within the callback function
    // Be careful as this can have unintended side effects (other iterate methods in this project don't do this)
    // Returning true from the callback breaks out of the iteration, false just continues normally
    void iterate(const std::function<bool(io::layouts::multiplayer_match&)> &callback);

}

#endif //SHIRO_MATCH_MANAGER_HH
