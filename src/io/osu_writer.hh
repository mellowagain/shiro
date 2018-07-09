#ifndef SHIRO_OSU_WRITER_HH
#define SHIRO_OSU_WRITER_HH

#include "osu_buffer.hh"
#include "layouts/layout.hh"

namespace shiro::io {

    class osu_writer {
    private:
        buffer data;

        buffer &write_packet(uint16_t id, layout data);

    public:
        osu_writer() = default;

        void login_reply(int32_t reply);
        void announce(std::string announcement);
        void protocol_negotiation(int32_t protocol_version);

        std::string serialize();

    };

}

#endif //SHIRO_OSU_WRITER_HH
