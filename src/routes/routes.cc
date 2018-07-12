#include "../config/bancho_file.hh"
#include "../logger/route_logger.hh"
#include "impl/root_route.hh"
#include "routes.hh"

void shiro::routes::init() {
    server.loglevel(crow::LogLevel::Info);
    crow::logger::setHandler(std::make_shared<logging::route_logger>().get());

    init_routes();

    server.concurrency(64); // osu! connections are keep alive so we should be able to handle them concurrently
    server.bindaddr(config::bancho::host).port((uint16_t) config::bancho::port).multithreaded().run();
}

void shiro::routes::init_routes() {
    CROW_ROUTE(server, "/").methods("GET"_method, "POST"_method)(root::handle);
}
