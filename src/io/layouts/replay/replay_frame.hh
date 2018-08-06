#ifndef SHIRO_REPLAY_FRAME_HH
#define SHIRO_REPLAY_FRAME_HH

#include "../serializable.hh"

namespace shiro::io::layouts {

    class replay_frame : public serializable {
    public:
        virtual ~replay_frame() = default;

        uint8_t button_state = 0;
        uint8_t button_type = 0;

        float mouse_x = 0.0f;
        float mouse_y = 0.0f;

        int32_t time = 0;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;
    };

}

#endif //SHIRO_REPLAY_FRAME_HH
