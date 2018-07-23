#ifndef SHIRO_BOT_HH
#define SHIRO_BOT_HH

#include <string>
#include <deque>

#include "../users/user.hh"

namespace shiro::bot {

    void init();

    void init_commands();

    bool handle(const std::string &command, std::deque<std::string> &args, std::shared_ptr<users::user> user, std::string channel);

}

#endif //SHIRO_BOT_HH
