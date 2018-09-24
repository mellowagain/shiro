#ifndef SHIRO_REPLAY_MANAGER_HH
#define SHIRO_REPLAY_MANAGER_HH

#include <string>

#include "../beatmaps/beatmap.hh"
#include "../scores/score.hh"

namespace shiro::replays {

    void init();

    void save_replay(const scores::score &s, const beatmaps::beatmap &beatmap, int32_t game_version, std::string replay);

    std::string calculate_diagram(const scores::score &s, std::string replay);

    bool has_replay(const scores::score &s);

}

#endif //SHIRO_REPLAY_MANAGER_HH
