#include "../config/bancho_file.hh"
#include "../thirdparty/loguru.hh"
#include "impl/root_route.hh"
#include "routes.hh"

void shiro::routes::init() {
    init_logger();
    init_routes();

    server.listen(config::bancho::host.c_str(), config::bancho::port);
}

void shiro::routes::init_routes() {
    server.Post("/", root::handle);
}

void shiro::routes::init_logger() {
    // Define meta data
    server.set_logger([](const httplib::Request &request, const httplib::Response &response) {
        LOG_F(INFO, "%s (0x%08x -> 0x%08x) %s %s %s",
              request.get_header_value("REMOTE_ADDR").c_str(),
              &request,
              &response,
              request.version.c_str(),
              request.method.c_str(),
              request.target.c_str()
        );
    });
}
