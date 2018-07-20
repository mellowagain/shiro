#ifndef SHIRO_PLAY_MODE_HH
#define SHIRO_PLAY_MODE_HH

#include <cstdint>

namespace shiro::utils {

    enum class play_mode : uint8_t {
        standard = 0,
        taiko = 1,
        fruits = 2,
        mania = 3

    };

}

#endif  // SHIRO_PLAY_MODE_HH
