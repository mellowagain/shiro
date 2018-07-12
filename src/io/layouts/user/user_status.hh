#ifndef SHIRO_USER_STATUS_HH
#define SHIRO_USER_STATUS_HH

#include "../layout.hh"

namespace shiro::io::layouts {

    class user_status : public layout {
    public:
        uint8_t status;
        std::string status_text;

        std::string beatmap_checksum;

        uint32_t current_mods;
        uint8_t play_mode;

        int32_t beatmap_id;

        std::string marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

    };

}

#endif //SHIRO_USER_STATUS_HH
