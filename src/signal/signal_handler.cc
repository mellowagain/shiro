#include "signal_handler.hh"

#if defined(_WIN32)
    #include "win32/windows_signal_handler.hh"
    namespace shi = shiro::signal_handler::win32;
#elif defined(__unix__) || defined(__linux__)
    #include "nix/nix_signal_handler.hh"
    namespace shi = shiro::signal_handler::nix;
#else
    #error Unsupported platform!
#endif

void shiro::signal_handler::install() {
    shi::install();
}
