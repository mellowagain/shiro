#include "../../thirdparty/loguru.hh"
#include "nix_signal_handler.hh"

#if defined(__unix__) || defined(__linux__)

#include <csignal>

void shiro::signal_handler::nix::install() {
    struct sigaction sig_int_handler;
    sig_int_handler.sa_handler = handle;
    sigemptyset(&sig_int_handler.sa_mask);
    sig_int_handler.sa_flags = 0;

    sigaction(SIGINT, &sig_int_handler, nullptr);

    LOG_S(INFO) << "Signal handler was successfully installed.";
}

void shiro::signal_handler::nix::handle(int signal) {
    if (signal != SIGINT)
        return;

    LOG_S(INFO) << "Shutting down...";
    std::exit(EXIT_SUCCESS);
}

#endif
