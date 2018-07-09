#include "../config/bancho_file.hh"
#include "impl/root_route.hh"
#include "routes.hh"

void shiro::routes::init() {
    // Define routes
    server.Get("/", root::handle);

    // Let the server listen
    server.listen(config::bancho::host.c_str(), config::bancho::port);
}
