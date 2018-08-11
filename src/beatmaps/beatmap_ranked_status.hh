#ifndef SHIRO_BEATMAP_RANKED_STATUS_HH
#define SHIRO_BEATMAP_RANKED_STATUS_HH

#include <cstdint>

namespace shiro::beatmaps {

    enum class status : int32_t {
        unknown = -2,
        unsubmitted = -1,
        latest_pending = 0,
        needs_update = 1,
        ranked = 2,
        approved = 3,
        qualified = 4,
        loved = 5

    };

}

#endif  // SHIRO_BEATMAP_RANKED_STATUS_HH
