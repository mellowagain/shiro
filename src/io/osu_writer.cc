#include <utility>

#include "layouts/packets.hh"
#include "osu_writer.hh"

shiro::io::buffer &shiro::io::osu_writer::write_packet(uint16_t id) {
    this->data.write_short(id);
    this->data.write_byte(0);
    this->data.write_int(0);

    return this->data;
}

template <typename l>
shiro::io::buffer &shiro::io::osu_writer::write_packet(uint16_t id, l data) {
    this->data.write_short(id);
    this->data.write_byte(0);
    this->data.write_string(data.marshal());

    return this->data;
}

void shiro::io::osu_writer::login_reply(int32_t reply) {
    this->write_packet((uint16_t) packet_id::out_login_reply, layout(reply));
}

void shiro::io::osu_writer::login_permissions(int32_t permissions) {
    this->write_packet((uint16_t) packet_id::out_login_permissions, layout(permissions));
}

void shiro::io::osu_writer::announce(std::string announcement) {
    this->write_packet((uint16_t) packet_id::out_announce, layout(std::move(announcement)));
}

void shiro::io::osu_writer::protocol_negotiation(int32_t protocol_version) {
    this->write_packet((uint16_t) packet_id::out_protocol_negotiation, layout(protocol_version));
}

void shiro::io::osu_writer::channel_listing_complete() {
    this->write_packet((uint16_t) packet_id::out_channel_listing_complete);
}

void shiro::io::osu_writer::channel_available(shiro::io::layouts::channel channel) {
    this->write_packet((uint16_t) packet_id::out_channel_available, std::move(channel));
}

void shiro::io::osu_writer::channel_join(std::string channel_name) {
    this->write_packet((uint16_t) packet_id::out_channel_join_success, layout(std::move(channel_name)));
}

std::string shiro::io::osu_writer::serialize() {
    return this->data.serialize();
}

shiro::io::buffer shiro::io::osu_writer::get_buffer() {
    return this->data;
}
