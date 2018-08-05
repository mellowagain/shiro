#ifndef SHIRO_ANNOUNCE_COMMAND_HH
#define SHIRO_ANNOUNCE_COMMAND_HH

#include <deque>
#include <string>

#include "../../../users/user.hh"

namespace shiro::bot::commands {

    bool announce(std::deque<std::string> &args, std::shared_ptr<users::user> user, std::string channel);

}

#endif //SHIRO_ANNOUNCE_COMMAND_HH
