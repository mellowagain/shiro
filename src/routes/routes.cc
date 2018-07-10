#include "../config/bancho_file.hh"
#include "impl/root_route.hh"
#include "routes.hh"

void shiro::routes::init() {
    init_routes();
    server
            .port((uint16_t) config::bancho::port)
            .multithreaded()
            .run();
}

void shiro::routes::init_routes() {
    CROW_ROUTE(server, "/").methods(crow::HTTPMethod::Post)(root::handle);
}
