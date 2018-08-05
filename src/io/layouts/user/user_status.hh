#ifndef SHIRO_USER_STATUS_HH
#define SHIRO_USER_STATUS_HH

#include "../serializable.hh"

namespace shiro::io::layouts {

    class user_status : public serializable {
    public:
        uint8_t activity = 0;
        std::string activity_desc = "";

        std::string beatmap_checksum = "";

        uint32_t current_mods = 0;
        uint8_t play_mode = 0;

        int32_t beatmap_id = 0;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;

    };

}

#endif //SHIRO_USER_STATUS_HH
