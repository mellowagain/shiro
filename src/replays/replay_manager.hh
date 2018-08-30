#ifndef SHIRO_REPLAY_MANAGER_HH
#define SHIRO_REPLAY_MANAGER_HH

#include <string>
#include "../scores/score.hh"

namespace shiro::replays {

    void init();

    void save_replay(const scores::score &s, int32_t game_version, std::string replay);

    bool has_replay(const scores::score &s);

}

#endif //SHIRO_REPLAY_MANAGER_HH
