#ifndef SHIRO_USER_PRESENCE_HH
#define SHIRO_USER_PRESENCE_HH

#include "../serializable.hh"

namespace shiro::io::layouts {

    class user_presence : public serializable {
    public:
        int32_t user_id = 0;
        std::string username = "";

        uint8_t time_zone = 0;
        uint8_t country_id = 0;

        uint8_t permissions = 5;

        float longitude = 0.0f;
        float latitude = 0.0f;

        int32_t rank = 0;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;

    };

}

#endif //SHIRO_USER_PRESENCE_HH
