#ifndef SHIRO_ROUTES_HH
#define SHIRO_ROUTES_HH

#include "../multipart/multiform_data_middleware.hh"
#include "../thirdparty/crow.hh"
#include "../shiro.hh"

namespace shiro::routes {

    extern crow::Crow<multipart::multiform_data_middleware> server;

    void init();

    void init_routes();

}

#endif //SHIRO_ROUTES_HH
