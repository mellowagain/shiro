#ifndef SHIRO_OSU_READER_HH
#define SHIRO_OSU_READER_HH

#include "osu_buffer.hh"
#include "osu_packet.hh"

namespace shiro::io {

    class osu_reader {
    private:
        buffer data;
        std::vector<osu_packet> packets;

    public:
        osu_reader() = default;
        explicit osu_reader(std::string data);

        std::vector<osu_packet> &parse();

    };

}

#endif //SHIRO_OSU_READER_HH
