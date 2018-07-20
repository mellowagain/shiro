#ifndef SHIRO_OSU_WRITER_HH
#define SHIRO_OSU_WRITER_HH

#include "layouts/channel/channel.hh"
#include "layouts/layout.hh"
#include "osu_buffer.hh"

namespace shiro::io {

    class osu_writer {
    private:
        buffer data;

        buffer &write_packet(uint16_t id);

        template <typename l>
        buffer &write_packet(uint16_t id, l data);

    public:
        osu_writer() = default;

        void login_reply(int32_t reply);
        void login_permissions(int32_t permissions);

        void announce(std::string announcement);

        void protocol_negotiation(int32_t protocol_version);

        void channel_listing_complete();
        void channel_available(layouts::channel channel);
        void channel_join(std::string channel_name);

        std::string serialize();

        buffer get_buffer();

    };

}

#endif //SHIRO_OSU_WRITER_HH
