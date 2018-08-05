#ifndef SHIRO_ROLL_COMMAND_HH
#define SHIRO_ROLL_COMMAND_HH

#include <deque>
#include <string>

#include "../../../users/user.hh"

namespace shiro::bot::commands {

    bool roll(std::deque<std::string> &args, std::shared_ptr<users::user> user, std::string channel);

}


#endif //SHIRO_ROLL_COMMAND_HH
