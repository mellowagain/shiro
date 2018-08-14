#ifndef SHIRO_ROUTES_HH
#define SHIRO_ROUTES_HH

#include "../thirdparty/crow.hh"
#include "../shiro.hh"

namespace shiro::routes {

    extern crow::Crow<> server;

    void init();

    void init_routes();

}

#endif //SHIRO_ROUTES_HH
