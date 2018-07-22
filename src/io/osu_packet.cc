#include "osu_packet.hh"

shiro::io::osu_packet::osu_packet(buffer &data) {
    this->id = (packet_id)data.read<uint16_t>();
    data.read<uint8_t>();

    int data_size = data.read<int32_t>();

    for (int i = 0; i < data_size; i++) {
        this->data.write<uint8_t>(static_cast<char>(data.read<uint8_t>()));
    }
}
