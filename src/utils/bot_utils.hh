#ifndef SHIRO_BOT_UTILS_HH
#define SHIRO_BOT_UTILS_HH

#include <string>

#include "../users/user.hh"

namespace shiro::utils::bot {

    void handle(io::layouts::message message, std::shared_ptr<users::user> user);

    void respond(std::string message, std::shared_ptr<users::user> user, std::string channel, bool only_user = false);

}

#endif //SHIRO_BOT_UTILS_HH
