#ifndef SHIRO_CONSOLE_OSU_BRIDGE_HH
#define SHIRO_CONSOLE_OSU_BRIDGE_HH

#include "../thirdparty/loguru.hh"

namespace shiro::channels::bridge {

    void install();

    void callback(void *user_data, const loguru::Message &message);

    uint8_t get_permission(loguru::Verbosity verbosity);

}

#endif //SHIRO_CONSOLE_OSU_BRIDGE_HH
