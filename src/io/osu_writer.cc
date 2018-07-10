#include <utility>

#include "layouts/packets.hh"
#include "osu_writer.hh"

shiro::io::buffer &shiro::io::osu_writer::write_packet(uint16_t id, shiro::io::layout data) {
    this->data.write_short(id);
    this->data.write_byte(0);
    this->data.write_string(data.marshal());

    return this->data;
}

void shiro::io::osu_writer::login_reply(int32_t reply) {
    this->write_packet((uint16_t) packet_id::out_login_reply, layout(reply));
}

void shiro::io::osu_writer::announce(std::string announcement) {
    this->write_packet((uint16_t) packet_id::out_announce, layout(std::move(announcement)));
}

void shiro::io::osu_writer::protocol_negotiation(int32_t protocol_version) {
    this->write_packet((uint16_t) packet_id::out_protocol_negotiation, layout(protocol_version));
}

std::string shiro::io::osu_writer::serialize() {
    return this->data.serialize();
}
