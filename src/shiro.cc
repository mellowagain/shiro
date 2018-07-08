#include <cstdlib>

#include "logger/logger.hh"
#include "thirdparty/loguru.hh"
#include "shiro.hh"

int shiro::init(int argc, char **argv) {
    logging::init(argc, argv);

    LOG_S(INFO) << "Welcome to shiro.";

    return EXIT_SUCCESS;
}

void shiro::destroy() {
    LOG_S(INFO) << "Thank you and goodbye.";
}
