#include "osu_reader.hh"

std::vector<shiro::io::osu_packet> shiro::io::parse_packets(std::string data) {
    std::vector<osu_packet> result;
    buffer buf(data);

    while (buf.can_read(7)) {
        result.emplace_back(osu_packet(buf));
    }

    return result;
}
