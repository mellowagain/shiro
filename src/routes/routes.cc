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

#include <boost/exception/diagnostic_information.hpp>

#include "../config/bancho_file.hh"
#include "../logger/route_logger.hh"
#include "../thirdparty/loguru.hh"
#include "impl/api/ci_trigger_route.hh"
#include "impl/web/bancho_connect_route.hh"
#include "impl/web/get_replay_route.hh"
#include "impl/web/get_scores_route.hh"
#include "impl/web/submit_score_route.hh"
#include "impl/root_route.hh"
#include "routes.hh"

crow::Crow<> shiro::routes::server;

void shiro::routes::init() {
    crow::logger::setHandler(std::make_shared<logging::route_logger>().get());
    server.loglevel(crow::LogLevel::Info);

    init_routes();

    try {
        server.multithreaded();
        server.concurrency(config::bancho::concurrency);

        server.bindaddr(config::bancho::host);
        server.port(config::bancho::port);

        server.run();
    } catch (const boost::system::system_error &ex) {
        logging::sentry::exception(ex);
        LOG_S(FATAL) << "Unable to start server: " << ex.what() << ".";
    }
}

void shiro::routes::init_routes() {
    CROW_ROUTE(server, "/").methods("GET"_method, "POST"_method)(shiro_route(root::handle));

    CROW_ROUTE(server, "/web/bancho_connect.php").methods("GET"_method)(shiro_route(web::bancho_connect::handle));
    CROW_ROUTE(server, "/web/osu-osz2-getscores.php").methods("GET"_method)(shiro_route(web::get_scores::handle));
    CROW_ROUTE(server, "/web/osu-getreplay.php").methods("GET"_method)(shiro_route(web::get_replay::handle));

    // Score submission routes
    // submit-modular.php: Legacy route with old table display, prior to b20181221.4
    // submit-modular-selector.php: Up-to-date route with new table display, after b20181221.4
    CROW_ROUTE(server, "/web/osu-submit-modular.php").methods("POST"_method)(shiro_route(web::submit_score::handle));
    CROW_ROUTE(server, "/web/osu-submit-modular-selector.php").methods("POST"_method)(shiro_route(web::submit_score::handle));

    // Non osu! routes
    CROW_ROUTE(server, "/api/ci_trigger").methods("POST"_method)(shiro_route(api::ci_trigger::handle));
}
