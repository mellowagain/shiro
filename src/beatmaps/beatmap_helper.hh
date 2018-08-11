#ifndef SHIRO_BEATMAP_HELPER_HH
#define SHIRO_BEATMAP_HELPER_HH

#include "beatmap_ranked_status.hh"

namespace shiro::beatmaps::helper {

    int32_t fix_beatmap_status(int32_t status_code);

    bool has_leaderboard(int32_t status_code);

}

#endif //SHIRO_BEATMAP_HELPER_HH
