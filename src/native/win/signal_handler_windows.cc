#if defined(_WIN32)

#include <cstdio>
#include <windows.h>

#include "../../thirdparty/loguru.hh"
#include "../signal_handler.hh"

bool handle_signal(unsigned int signal) {
    switch (signal) {
        case CTRL_C_EVENT:
            LOG_S(INFO) << "Shutting down...";
            std::exit(EXIT_SUCCESS);
        case CTRL_CLOSE_EVENT:
            return true;
        default:
            return false;
    }
}

void shiro::native::signal_handler::install() {
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE) handle_signal, true))
        LOG_S(FATAL) << "Unable to install signal handler.";

    LOG_S(INFO) << "Signal handler was successfully installed.";
}

#endif
