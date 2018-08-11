#ifndef SHIRO_MULTIFORM_DATA_MIDDLEWARE_HH
#define SHIRO_MULTIFORM_DATA_MIDDLEWARE_HH

#include "../thirdparty/crow.hh"

namespace shiro::multipart {

    struct multiform_data_middleware {

        struct context {
            // Empty
        };

        void before_handle(crow::request &req, crow::response &res, context &ctx);

        void after_handle(crow::request &req, crow::response &res, context &ctx);

    };

}

#endif //SHIRO_MULTIFORM_DATA_MIDDLEWARE_HH
