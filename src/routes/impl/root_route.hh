#ifndef SHIRO_ROOT_ROUTE_HH
#define SHIRO_ROOT_ROUTE_HH

#include "../../thirdparty/crow.hh"

namespace shiro::routes::root {

    crow::response handle(const crow::request &request);

}

#endif //SHIRO_ROOT_ROUTE_HH
