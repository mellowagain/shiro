#include "osu_writer.hh"

void shiro::io::osu_writer::login_reply(int32_t reply) {
    this->write(packet_id::out_login_reply, serializable(reply));
}

void shiro::io::osu_writer::login_permissions(int32_t permissions) {
    this->write(packet_id::out_login_permissions, serializable(permissions));
}

void shiro::io::osu_writer::user_quit(shiro::io::layouts::user_quit user_quit) {
    this->write(packet_id::out_handle_user_quit, std::move(user_quit));
}

void shiro::io::osu_writer::announce(std::string announcement) {
    this->write(packet_id::out_announce, serializable(std::move(announcement)));
}

void shiro::io::osu_writer::protocol_negotiation(int32_t protocol_version) {
    this->write(packet_id::out_protocol_negotiation, serializable(protocol_version));
}

void shiro::io::osu_writer::channel_listing_complete() {
    this->write(packet_id::out_channel_listing_complete);
}

void shiro::io::osu_writer::channel_available(shiro::io::layouts::channel channel) {
    this->write(packet_id::out_channel_available, std::move(channel));
}

void shiro::io::osu_writer::channel_join(std::string channel_name) {
    this->write(packet_id::out_channel_join_success, serializable(std::move(channel_name)));
}

void shiro::io::osu_writer::channel_revoked(std::string channel_name) {
    this->write(packet_id::out_channel_revoked, serializable(std::move(channel_name)));
}

void shiro::io::osu_writer::user_presence(shiro::io::layouts::user_presence presence) {
    this->write(packet_id::out_user_presence, std::move(presence));
}

void shiro::io::osu_writer::user_stats(shiro::io::layouts::user_stats stats) {
    this->write(packet_id::out_handle_osu_update, std::move(stats));
}

void shiro::io::osu_writer::user_silenced(int32_t id) {
    this->write(packet_id::out_user_silenced, serializable(id));
}

void shiro::io::osu_writer::friend_list(std::vector<int32_t> friends) {
    this->write(packet_id::out_friends_list, serializable(std::move(friends)));
}

void shiro::io::osu_writer::users_list(std::vector<int32_t> users) {
    this->write(packet_id::out_user_presence_bundle, serializable(std::move(users)));
}

void shiro::io::osu_writer::send_message(shiro::io::layouts::message message) {
    this->write(packet_id::out_send_message, std::move(message));
}

void shiro::io::osu_writer::rtx(std::string rtx) {
    this->write(packet_id::out_rtx, serializable(std::move(rtx)));
}

void shiro::io::osu_writer::spectator_join_user(int32_t id) {
    this->write(packet_id::out_fellow_spectator_joined, serializable(id));
}

void shiro::io::osu_writer::spectator_join_host(int32_t id) {
    this->write(packet_id::out_spectator_joined, serializable(id));
}

void shiro::io::osu_writer::spectator_left_user(int32_t id) {
    this->write(packet_id::out_fellow_spectator_left, serializable(id));
}

void shiro::io::osu_writer::spectator_left_host(int32_t id) {
    this->write(packet_id::out_spectator_left, serializable(id));
}

void shiro::io::osu_writer::spectator_cant_spectate(int32_t id) {
    this->write(packet_id::out_spectator_cant_spectate, serializable(id));
}

void shiro::io::osu_writer::spectate_frames(layouts::spectate_frames frames) {
    this->write(packet_id::out_spectate_frames, std::move(frames));
}

std::string shiro::io::osu_writer::serialize() {
    return this->buf.serialize();
}

shiro::io::buffer &shiro::io::osu_writer::get_buffer() {
    return this->buf;
}
