#ifndef SHIRO_OSU_READER_HH
#define SHIRO_OSU_READER_HH

#include "osu_buffer.hh"
#include "osu_packet.hh"

namespace shiro::io {

    std::vector<osu_packet> parse_packets(std::string data);

}

#endif //SHIRO_OSU_READER_HH
