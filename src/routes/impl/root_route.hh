#ifndef SHIRO_ROOT_ROUTE_HH
#define SHIRO_ROOT_ROUTE_HH

#include "../../thirdparty/crow.hh"

namespace shiro::routes::root {

    void handle(const crow::request &request, crow::response &response);

}

#endif //SHIRO_ROOT_ROUTE_HH
