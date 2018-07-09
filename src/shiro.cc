#include <cstdlib>

#include "config/cli_args.hh"
#include "config/db_file.hh"
#include "logger/logger.hh"
#include "shiro.hh"
#include "thirdparty/cli11.hh"
#include "thirdparty/loguru.hh"

int shiro::init(int argc, char **argv) {
    logging::init(argc, argv);

    config::cli::parse(argc, argv);
    config::database::parse();

    db_connection = std::make_shared<database>(
            config::database::address, config::database::port, config::database::database, config::database::username, config::database::password
    );
    db_connection->connect();
    db_connection->setup();

    LOG_S(INFO) << "Welcome to shiro.";

    return EXIT_SUCCESS;
}

void shiro::destroy() {
    if (db_connection != nullptr)
        db_connection->disconnect();

    LOG_S(INFO) << "Thank you and goodbye.";
}
