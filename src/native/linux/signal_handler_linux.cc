#if defined(__linux__) || defined(__APPLE__) // This is a exception because the signal handlers are the same on *nix

#include <csignal>

#include "../../thirdparty/loguru.hh"
#include "../signal_handler.hh"

void handle_signal(int signal) {
    if (signal != SIGINT)
        return;

    LOG_S(INFO) << "Shutting down...";
    std::exit(EXIT_SUCCESS);
}

void shiro::native::signal_handler::install() {
    struct sigaction sig_int_handler;
    sig_int_handler.sa_handler = handle_signal;
    sigemptyset(&sig_int_handler.sa_mask);
    sig_int_handler.sa_flags = 0;

    sigaction(SIGINT, &sig_int_handler, nullptr);

    LOG_S(INFO) << "Signal handler was successfully installed.";
}

#endif
