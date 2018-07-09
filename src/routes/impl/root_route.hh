#ifndef SHIRO_ROOT_ROUTE_HH
#define SHIRO_ROOT_ROUTE_HH

#include "../../thirdparty/httplib.hh"

namespace shiro::routes::root {

    void handle(const httplib::Request &request, httplib::Response &response);

}

#endif //SHIRO_ROOT_ROUTE_HH
