#ifndef SHIRO_OSU_PACKET_HH
#define SHIRO_OSU_PACKET_HH

#include "osu_buffer.hh"
#include "layouts/packets.hh"

namespace shiro::io {

    class osu_packet {
    public:
        packet_id id;
        buffer data;

        explicit osu_packet(buffer &data);

        template <typename l>
        l unmarshal();

    };

}

#endif //SHIRO_OSU_PACKET_HH
