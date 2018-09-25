/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018 Marc3842h, czapek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

void shiro::io::osu_writer::user_ban_info(int32_t duration) {
    this->write(packet_id::out_ban_info, serializable(duration));
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
