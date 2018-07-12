#ifndef SHIRO_LOGIN_HANDLER_HH
#define SHIRO_LOGIN_HANDLER_HH

#include "../thirdparty/crow.hh"

namespace shiro::handler::login {

    void handle(const crow::request &request, crow::response &response);

}

#endif //SHIRO_LOGIN_HANDLER_HH
