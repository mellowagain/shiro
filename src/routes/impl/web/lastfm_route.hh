#ifndef SHIRO_LASTFM_ROUTE_HH
#define SHIRO_LASTFM_ROUTE_HH

#include "../../../thirdparty/crow.hh"

namespace shiro::routes::web::lastfm {

    void handle(const crow::request &request, crow::response &response);

}

#endif //SHIRO_LASTFM_ROUTE_HH
