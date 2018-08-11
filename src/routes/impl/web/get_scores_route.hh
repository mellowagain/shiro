#ifndef SHIRO_GET_SCORES_ROUTE_HH
#define SHIRO_GET_SCORES_ROUTE_HH

#include "../../../thirdparty/crow.hh"

namespace shiro::routes::web::get_scores {

    void handle(const crow::request &request, crow::response &response);

}

#endif //SHIRO_GET_SCORES_ROUTE_HH
