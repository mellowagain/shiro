#ifndef SHIRO_ROUTES_HH
#define SHIRO_ROUTES_HH

#include "../shiro.hh"
#include "../thirdparty/crow.hh"

namespace shiro::routes {

    static crow::Crow<> server;

    void init();

    void init_routes();

}

#endif //SHIRO_ROUTES_HH
