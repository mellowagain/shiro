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

#ifndef SHIRO_USER_MANAGER_HH
#define SHIRO_USER_MANAGER_HH

#include <memory>
#include <vector>

#include "user.hh"

namespace shiro::users::manager {

    extern std::vector<std::shared_ptr<user>> online_users;

    // Adds a user to online users
    void login_user(std::shared_ptr<user> user);

    // Removes a user from online users
    void logout_user(std::shared_ptr<user> user);
    void logout_user(int32_t user_id);

    bool is_online(std::shared_ptr<user> user);
    bool is_online(int32_t user_id);
    bool is_online(const std::string &token);

    std::shared_ptr<user> get_user_by_username(const std::string &username);
    std::shared_ptr<user> get_user_by_id(int32_t id);
    std::shared_ptr<user> get_user_by_token(const std::string &token);

    // Works for both online and offline users
    std::string get_username_by_id(int32_t id);
    int32_t get_id_by_username(const std::string &username);

    size_t get_online_users();

}

#endif //SHIRO_USER_MANAGER_HH
