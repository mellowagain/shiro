#ifndef SHIRO_PUNISHMENT_TYPE_HH
#define SHIRO_PUNISHMENT_TYPE_HH

#include <cstdint>

namespace shiro::utils {

    enum class punishment_type : uint16_t {
        kick = 0,
        silence = 1,
        restrict = 2,
        ban = 3
    };

}

#endif  // SHIRO_PUNISHMENT_TYPE_HH
