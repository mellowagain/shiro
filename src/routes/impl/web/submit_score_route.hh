#ifndef SHIRO_SUBMIT_SCORE_ROUTE_HH
#define SHIRO_SUBMIT_SCORE_ROUTE_HH

#include "../../../thirdparty/crow.hh"

namespace shiro::routes::web::submit_score {

    void handle(const crow::request &request, crow::response &response);

}

#endif //SHIRO_SUBMIT_SCORE_ROUTE_HH
