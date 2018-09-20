#ifndef SHIRO_BANCHO_FILE_HH
#define SHIRO_BANCHO_FILE_HH

#include <string>

namespace shiro::config::bancho {

    extern std::string host;
    extern uint16_t port;
    extern uint16_t concurrency;

    extern std::string api_key;

    void parse();

}

#endif //SHIRO_BANCHO_FILE_HH
