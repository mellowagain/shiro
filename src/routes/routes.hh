#ifndef SHIRO_ROUTES_HH
#define SHIRO_ROUTES_HH

#include "../thirdparty/httplib.hh"
#include "../shiro.hh"

namespace shiro::routes {

    static inline httplib::Server server;

    void init();

    void init_routes();

    void init_logger();

}

#endif //SHIRO_ROUTES_HH
