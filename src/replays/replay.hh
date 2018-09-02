#ifndef SHIRO_REPLAY_HH
#define SHIRO_REPLAY_HH

#include <string>
#include <vector>

#include "../scores/score.hh"

namespace shiro::replays {

    class action {
    public:
        int64_t w = 0;
        float x = 0.0f;
        float y = 0.0f;
        int32_t z = 0; // buttons

        action() = default;

        std::string to_string() const;

    };

    class replay {
    private:
        scores::score score;
        std::vector<action> actions;
        std::string raw_replay;

    public:
        replay(scores::score s, std::string replay);

        void parse();

        std::vector<action> get_actions();

        std::string to_string() const;

    };

}

#endif //SHIRO_REPLAY_HH
