/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2020 Marc3842h, czapek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SHIRO_ROUTES_HH
#define SHIRO_ROUTES_HH

#include "../logger/sentry_logger.hh"
#include "../thirdparty/crow.hh"
#include "../shiro.hh"

namespace shiro::routes {

    extern crow::Crow<> server;

    void init();

    void init_routes();

}

#define shiro_route(handler)                                                                                                        \
    [](const crow::request &request, crow::response &response) {                                                                    \
        logging::sentry::http_request_in(request);                                                                                  \
                                                                                                                                    \
        try {                                                                                                                       \
            handler(request, response);                                                                                             \
        } catch (...) {                                                                                                             \
            logging::sentry::exception(std::current_exception());                                                                   \
            LOG_F(ERROR, "A exception occurred in %s: %s", #handler, boost::current_exception_diagnostic_information().c_str());    \
                                                                                                                                    \
            response.code = 500;                                                                                                    \
            response.end();                                                                                                         \
        }                                                                                                                           \
    }                                                                                                                               \

// Currently only allows for one parameter but we can expand upon this when needed
#define shiro_route_parameterized(handler, type, param)                                                                             \
    [](const crow::request &request, crow::response &response, type param) {                                                        \
        logging::sentry::http_request_in(request);                                                                                  \
                                                                                                                                    \
        try {                                                                                                                       \
            handler(request, response, param);                                                                                      \
        } catch (...) {                                                                                                             \
            logging::sentry::exception(std::current_exception());                                                                   \
            LOG_F(ERROR, "A exception occurred in %s: %s", #handler, boost::current_exception_diagnostic_information().c_str());    \
                                                                                                                                    \
            response.code = 500;                                                                                                    \
            response.end();                                                                                                         \
        }                                                                                                                           \
    }                                                                                                                               \

#endif //SHIRO_ROUTES_HH
