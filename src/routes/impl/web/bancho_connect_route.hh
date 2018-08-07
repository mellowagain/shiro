#ifndef SHIRO_BANCHO_CONNECT_ROUTE_HH
#define SHIRO_BANCHO_CONNECT_ROUTE_HH

#include "../../../thirdparty/crow.hh"

namespace shiro::routes::web::bancho_connect {

    void handle(const crow::request &request, crow::response &response);

}

#endif //SHIRO_BANCHO_CONNECT_ROUTE_HH
