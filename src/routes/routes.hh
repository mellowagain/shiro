/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018 Marc3842h, czapek
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
