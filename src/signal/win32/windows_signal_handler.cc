#include "../../thirdparty/loguru.hh"
#include "windows_signal_handler.hh"
#if defined(_WIN32)

#include <cstdio>
#include <windows.h>

void shiro::signal_handler::win32::install() {
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE) handle, true))
        LOG_S(FATAL) << "Unable to install signal handler.";

    LOG_S(INFO) << "Signal handler was successfully installed.";
}

bool shiro::signal_handler::win32::handle(unsigned int signal) {
    switch (signal) {
        case CTRL_C_EVENT:
            LOG_S(INFO) << "Shutting down...";
            std::exit(EXIT_SUCCESS);
            return true;
        case CTRL_CLOSE_EVENT:
            return true;
        default:
            return false;
    }
}

#endif
