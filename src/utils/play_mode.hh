#ifndef SHIRO_PLAY_MODE_HH
#define SHIRO_PLAY_MODE_HH

#include <cstdint>
#include <string>

namespace shiro::utils {

    enum class play_mode : uint8_t {
        standard = 0,
        taiko = 1,
        fruits = 2,
        mania = 3

    };

    std::string play_mode_to_string(play_mode mode);

}

#endif  //SHIRO_PLAY_MODE_HH
