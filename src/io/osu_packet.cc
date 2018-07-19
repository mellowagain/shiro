#include "layouts/layout.hh"
#include "osu_packet.hh"

shiro::io::osu_packet::osu_packet(buffer &data) {
    this->id = (packet_id) data.read_short();
    data.read_byte();

    int data_size = data.read_int();

    for (int i = 0; i < data_size; i++) {
        this->data.write_byte(static_cast<char>(data.read_byte()));
    }
}
