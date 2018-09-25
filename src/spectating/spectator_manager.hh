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

#ifndef SHIRO_SPECTATOR_MANAGER_HH
#define SHIRO_SPECTATOR_MANAGER_HH

#include <memory>

#include "../users/user.hh"

namespace shiro::spectating::manager {

    extern std::vector<std::pair<std::shared_ptr<users::user>, std::shared_ptr<users::user>>> currently_spectating;

    void start_spectating(std::shared_ptr<users::user> user, std::shared_ptr<users::user> target);

    void stop_spectating(std::shared_ptr<users::user> user);

    bool is_spectating(std::shared_ptr<users::user> user);

    bool is_being_spectated(std::shared_ptr<users::user> user);

    std::vector<std::shared_ptr<users::user>> get_spectators(std::shared_ptr<users::user> user);

    std::shared_ptr<users::user> get_host(std::shared_ptr<users::user> user);

}

#endif //SHIRO_SPECTATOR_MANAGER_HH
