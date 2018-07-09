#ifndef SHIRO_NIX_SIGNAL_HANDLER_HH
#define SHIRO_NIX_SIGNAL_HANDLER_HH
#if defined(__unix__) || defined(__linux__)

namespace shiro::signal_handler::nix {

    void install();

    void handle(int signal);

}

#endif
#endif //SHIRO_NIX_SIGNAL_HANDLER_HH
