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

    LOG_S(INFO) << "Welcome to shiro.";

    return EXIT_SUCCESS;
}

void shiro::destroy() {
    LOG_S(INFO) << "Thank you and goodbye.";
}
