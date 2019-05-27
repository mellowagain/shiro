#ifndef SHIRO_DIRECT_FILE_HH
#define SHIRO_DIRECT_FILE_HH

#include <cstdint>
#include <string>

namespace shiro::config::direct {

    extern bool enabled;
    extern int32_t provider;

    // Provider 0: Shirogane
    extern uint64_t shm;

    // Provider 1: Emulation
    extern std::string base_url;
    extern std::string mirror_url;

    // Provider 2: Beatconnect
    extern std::string api_key;

    void parse();

}

#endif //SHIRO_DIRECT_FILE_HH
