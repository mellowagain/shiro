#include <algorithm>

#include "../thirdparty/loguru.hh"
#include "user_manager.hh"

void shiro::users::manager::login_user(std::shared_ptr<shiro::users::user> user) {
    if (user == nullptr || user->token.empty())
        return;

    if (is_online(user))
        logout_user(user);

    online_users.emplace_back(user);

    LOG_F(INFO, "User %s logged in successfully.", user->presence.username.c_str());
}

void shiro::users::manager::logout_user(std::shared_ptr<shiro::users::user> user) {
    if (user == nullptr || !is_online(user))
        return;

    auto iterator = std::find(online_users.begin(), online_users.end(), user);

    if (iterator == online_users.end())
        return;

    ptrdiff_t index = std::distance(online_users.begin(), iterator);
    online_users.erase(online_users.begin() + index);

    LOG_F(INFO, "User %s logged out successfully.", user->presence.username.c_str());
}

void shiro::users::manager::logout_user(int32_t user_id) {
    if (!is_online(user_id))
        return;

    std::shared_ptr<user> target_user = nullptr;

    for (const std::shared_ptr<user> &user : online_users) {
        if (user->user_id == user_id) {
            target_user = user;
            break;
        }
    }

    if (target_user == nullptr)
        return;

    logout_user(target_user);
}

bool shiro::users::manager::is_online(std::shared_ptr<shiro::users::user> user) {
    if (user == nullptr)
        return false;

    return std::find(online_users.begin(), online_users.end(), user) != online_users.end();
}

bool shiro::users::manager::is_online(int32_t user_id) {
    for (const std::shared_ptr<user> &user : online_users) {
        if (user->user_id == user_id)
            return true;
    }

    return false;
}

bool shiro::users::manager::is_online(const std::string &token) {
    if (token.empty())
        return false;

    for (const std::shared_ptr<user> &user : online_users) {
        if (user->token == token)
            return true;
    }

    return false;
}

std::shared_ptr<shiro::users::user> shiro::users::manager::get_user_by_username(const std::string &username) {
    if (username.empty())
        return nullptr;

    for (const std::shared_ptr<user> &user : online_users) {
        if (user->presence.username == username)
            return user;
    }

    return nullptr;
}

std::shared_ptr<shiro::users::user> shiro::users::manager::get_user_by_id(int32_t id) {
    if (!is_online(id))
        return nullptr;

    for (const std::shared_ptr<user> &user : online_users) {
        if (user->user_id == id)
            return user;
    }

    return nullptr;
}

std::shared_ptr<shiro::users::user> shiro::users::manager::get_user_by_token(const std::string &token) {
    if (token.empty())
        return nullptr;

    if (!is_online(token))
        return nullptr;

    for (const std::shared_ptr<user> &user : online_users) {
        if (user->token == token)
            return user;
    }

    return nullptr;
}

size_t shiro::users::manager::get_online_users() {
    return online_users.size();
}
