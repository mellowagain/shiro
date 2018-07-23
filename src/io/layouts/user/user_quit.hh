#ifndef SHIRO_USER_QUIT_HH
#define SHIRO_USER_QUIT_HH

#include "../serializable.hh"

namespace shiro::io::layouts {

    class user_quit : public serializable {
    public:
        int32_t user_id;
        uint8_t state;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;

    };

}

#endif //SHIRO_USER_QUIT_HH
