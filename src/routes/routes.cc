#include "../config/bancho_file.hh"
#include "../logger/route_logger.hh"
#include "../thirdparty/loguru.hh"
#include "impl/web/bancho_connect_route.hh"
#include "impl/web/get_scores_route.hh"
#include "impl/web/submit_score_route.hh"
#include "impl/root_route.hh"
#include "routes.hh"

crow::Crow<> shiro::routes::server;

void shiro::routes::init() {
    server.loglevel(crow::LogLevel::Info);
    crow::logger::setHandler(std::make_shared<logging::route_logger>().get());

    init_routes();

    try {
        server.concurrency(config::bancho::concurrency);
        server.bindaddr(config::bancho::host).port((uint16_t) config::bancho::port).multithreaded().run();
    } catch (const boost::system::system_error &ex) {
        LOG_S(FATAL) << "Unable to start server: " << ex.what() << ".";
    }
}

void shiro::routes::init_routes() {
    CROW_ROUTE(server, "/").methods("GET"_method, "POST"_method)(root::handle);

    CROW_ROUTE(server, "/web/bancho_connect.php").methods("GET"_method)(web::bancho_connect::handle);
    CROW_ROUTE(server, "/web/osu-osz2-getscores.php").methods("GET"_method)(web::get_scores::handle);
    CROW_ROUTE(server, "/web/osu-submit-modular.php").methods("POST"_method)(web::submit_score::handle);
}
