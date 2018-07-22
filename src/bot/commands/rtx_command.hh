#ifndef SHIRO_RTX_COMMAND_HH
#define SHIRO_RTX_COMMAND_HH

#include <deque>
#include <string>

#include "../../users/user.hh"

namespace shiro::bot::commands {

    bool rtx(std::deque<std::string> &args, std::shared_ptr<users::user> user);

}

#endif //SHIRO_RTX_COMMAND_HH
