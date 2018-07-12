#ifndef SHIRO_USER_MANAGER_HH
#define SHIRO_USER_MANAGER_HH

#include <memory>
#include <vector>

#include "user.hh"

namespace shiro::users::manager {

    static std::vector<std::shared_ptr<user>> online_users;

    // Adds a user to online users
    void login_user(std::shared_ptr<user> user);

    // Removes a user from online users
    void logout_user(std::shared_ptr<user> user);
    void logout_user(int32_t user_id);

    bool is_online(std::shared_ptr<user> user);
    bool is_online(int32_t user_id);
    bool is_online(const std::string &token);

    std::shared_ptr<user> get_user_by_token(const std::string &token);

    size_t get_online_users();

}

#endif //SHIRO_USER_MANAGER_HH
