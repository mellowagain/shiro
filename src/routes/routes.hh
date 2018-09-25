#ifndef SHIRO_ROUTES_HH
#define SHIRO_ROUTES_HH

#include "../thirdparty/crow.hh"
#include "../shiro.hh"

namespace shiro::routes {

    extern crow::Crow<> server;

    void init();

    void init_routes();

}

#define shiro_route(handler)                                                                                                \
    [](const crow::request &request, crow::response &response) {                                                            \
        try {                                                                                                               \
            handler(request, response);                                                                                     \
        } catch (...) {                                                                                                     \
            LOG_S(ERROR) << "A exception occurred in " #handler ": " << boost::current_exception_diagnostic_information();  \
                                                                                                                            \
            response.code = 500;                                                                                            \
            response.end();                                                                                                 \
        }                                                                                                                   \
    }                                                                                                                       \

#endif //SHIRO_ROUTES_HH
