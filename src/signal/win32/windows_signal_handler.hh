#ifndef SHIRO_WINDOWS_SIGNAL_HANDLER_HH
#define SHIRO_WINDOWS_SIGNAL_HANDLER_HH
#if defined(_WIN32)

namespace shiro::signal_handler::win32 {

    void install();

    bool handle(unsigned int signal);

}

#endif
#endif //SHIRO_WINDOWS_SIGNAL_HANDLER_HH
