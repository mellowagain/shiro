#ifndef SHIRO_SPECTATE_FRAMES_HH
#define SHIRO_SPECTATE_FRAMES_HH

#include <vector>

#include "../serializable.hh"
#include "replay_frame.hh"
#include "score_frame.hh"

namespace shiro::io::layouts {

    class spectate_frames : public serializable {
    public:
        int32_t extra = 0;
        std::vector<replay_frame> replay_frames;
        uint8_t action = 0;
        layouts::score_frame score_frame;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;
    };

}

#endif //SHIRO_SPECTATE_FRAMES_HH
