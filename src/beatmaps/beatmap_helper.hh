#ifndef SHIRO_BEATMAP_HELPER_HH
#define SHIRO_BEATMAP_HELPER_HH

#include <cstdio>
#include <string>

#include "beatmap_ranked_status.hh"

namespace shiro::beatmaps::helper {

    void init();

    int32_t fix_beatmap_status(int32_t status_code);

    bool has_leaderboard(int32_t status_code);

    bool awards_pp(int32_t status_code);

    size_t callback(void *raw_data, size_t size, size_t memory, std::string *ptr);

    FILE *download(int32_t beatmap_id);

}

#endif //SHIRO_BEATMAP_HELPER_HH
