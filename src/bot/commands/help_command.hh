#ifndef SHIRO_HELP_COMMAND_HH
#define SHIRO_HELP_COMMAND_HH

#include <deque>
#include <string>

#include "../../users/user.hh"

namespace shiro::bot::commands {

    bool help(std::deque<std::string> &args, std::shared_ptr<users::user> user, std::string channel);

}

#endif //SHIRO_HELP_COMMAND_HH
