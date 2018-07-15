#include <cstdlib>
#include <curl/curl.h>

#include "channels/channel_manager.hh"
#include "config/bancho_file.hh"
#include "config/cli_args.hh"
#include "config/db_file.hh"
#include "config/geonames_file.hh"
#include "geoloc/country_ids.hh"
#include "logger/logger.hh"
#include "routes/routes.hh"
#include "signal/signal_handler.hh"
#include "thirdparty/cli11.hh"
#include "thirdparty/loguru.hh"
#include "shiro.hh"

std::shared_ptr<shiro::database> shiro::db_connection = nullptr;

int shiro::init(int argc, char **argv) {
    logging::init(argc, argv);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    config::cli::parse(argc, argv);

    config::bancho::parse();
    config::database::parse();
    config::geonames::parse();

    geoloc::init();

    db_connection = std::make_shared<database>(
            config::database::address, config::database::port, config::database::database,
            config::database::username, config::database::password
    );
    db_connection->connect();
    db_connection->setup();

    channels::manager::init();
    shiro::signal_handler::install();

    LOG_F(INFO, "Welcome to shiro. Listening on http://%s:%i/.", config::bancho::host.c_str(), config::bancho::port);
    LOG_F(INFO, "Press CTRL + C to quit.");

    shiro::routes::init();

    return EXIT_SUCCESS;
}

void shiro::destroy() {
    LOG_S(INFO) << "Thank you and goodbye.";
}
