#ifndef SHIRO_USER_PRESENCE_HH
#define SHIRO_USER_PRESENCE_HH

#include "../serializable.hh"

namespace shiro::io::layouts {

    class user_presence : public serializable {
    public:
        int32_t user_id;
        std::string username;

        uint8_t time_zone;
        uint8_t country_id;

        uint8_t permissions;

        float longitude;
        float latitude;

        int32_t rank;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;

    };

}

#endif //SHIRO_USER_PRESENCE_HH
