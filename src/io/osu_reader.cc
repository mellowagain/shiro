#include "osu_reader.hh"

shiro::io::osu_reader::osu_reader(std::string data) {
    this->data = buffer(data);
}

std::vector<shiro::io::osu_packet> &shiro::io::osu_reader::parse() {
    while (this->data.can_read(7)) {
        this->packets.emplace_back(osu_packet(this->data));
    }

    return this->packets;
}
