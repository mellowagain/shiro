#include "layouts/layout.hh"
#include "osu_packet.hh"

shiro::io::osu_packet::osu_packet(buffer &data) {
    this->id = (packet_id) data.read_short();
    data.read_byte();

    int data_size = data.read_int();
    this->data.write_int(data_size);

    for (int i = 0; i < data_size; i++) {
        this->data.write_byte(data.read_byte());
    }
}

template<typename l = shiro::io::layout>
l shiro::io::osu_packet::unmarshal() {
    l result;
    result.unmarshal(this->data);

    return result;
}
