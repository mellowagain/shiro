/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2019 Marc3842h, czapek
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

#include "../handlers/multiplayer/lobby/lobby_leave_handler.hh"
#include "../handlers/multiplayer/lobby/lobby_join_handler.hh"
#include "../handlers/multiplayer/room/room_change_host_handler.hh"
#include "../handlers/multiplayer/room/room_change_password_handler.hh"
#include "../handlers/multiplayer/room/room_create_handler.hh"
#include "../handlers/multiplayer/room/room_join_handler.hh"
#include "../handlers/multiplayer/room/room_leave_handler.hh"
#include "../handlers/chat/leave_channel_handler.hh"
#include "../handlers/chat/join_channel_handler.hh"
#include "../handlers/chat/private_chat_handler.hh"
#include "../handlers/chat/public_chat_handler.hh"
#include "../handlers/friends/friend_add_handler.hh"
#include "../handlers/friends/friend_remove_handler.hh"
#include "../handlers/presence/user_presence_request_all_handler.hh"
#include "../handlers/presence/user_presence_request_handler.hh"
#include "../handlers/presence/user_stats_request_handler.hh"
#include "../handlers/spectating/start_spectating_handler.hh"
#include "../handlers/spectating/stop_spectating_handler.hh"
#include "../handlers/spectating/spectator_frames_handler.hh"
#include "../handlers/spectating/cant_spectate_handler.hh"
#include "../handlers/logout_handler.hh"
#include "../handlers/ping_handler.hh"
#include "../handlers/request_status_update_handler.hh"
#include "../handlers/user_status_handler.hh"
#include "../thirdparty/loguru.hh"
#include "packet_router.hh"

void shiro::routes::route(shiro::io::packet_id packet_id, shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    switch (packet_id) {
        case io::packet_id::in_send_user_status:
            handler::user_status::handle(in, out, user);
            break;
        case io::packet_id::in_send_irc_message:
            handler::chat::handle_public(in, out, user);
            break;
        case io::packet_id::in_exit:
            handler::logout::handle(in, out, user);
            break;
        case io::packet_id::in_request_status_update:
            handler::request_status_update::handle(in, out, user);
            break;
        case io::packet_id::in_pong:
            handler::ping::handle(in, out, user);
            break;
        case io::packet_id::in_start_spectating:
            handler::spectating::start::handle(in, out, user);
            break;
        case io::packet_id::in_stop_spectating:
            handler::spectating::stop::handle(in, out, user);
            break;
        case io::packet_id::in_spectate_frames:
            handler::spectating::frames::handle(in, out, user);
            break;
        case io::packet_id::in_error_report:break;
        case io::packet_id::in_cant_spectate:
            handler::spectating::cant_spectate::handle(in, out, user);
            break;
        case io::packet_id::in_send_irc_message_private:
            handler::chat::handle_private(in, out, user);
            break;
        case io::packet_id::in_lobby_part:
            handler::multiplayer::lobby::leave::handle(in, out, user);
            break;
        case io::packet_id::in_lobby_join:
            handler::multiplayer::lobby::join::handle(in, out, user);
            break;
        case io::packet_id::in_match_create:
            handler::multiplayer::room::create::handle(in, out, user);
            break;
        case io::packet_id::in_match_join:
            handler::multiplayer::room::join::handle(in, out, user);
            break;
        case io::packet_id::in_match_part:
            handler::multiplayer::room::leave::handle(in, out, user);
            break;
        case io::packet_id::in_match_change_slot:break;
        case io::packet_id::in_match_ready:break;
        case io::packet_id::in_match_lock:break;
        case io::packet_id::in_match_change_settings:break;
        case io::packet_id::in_match_start:break;
        case io::packet_id::in_match_score_update:break;
        case io::packet_id::in_match_complete:break;
        case io::packet_id::in_match_change_mods:break;
        case io::packet_id::in_match_load_complete:break;
        case io::packet_id::in_match_no_beatmap:break;
        case io::packet_id::in_match_not_ready:break;
        case io::packet_id::in_match_failed:break;
        case io::packet_id::in_match_has_beatmap:break;
        case io::packet_id::in_match_skip_request:break;
        case io::packet_id::in_channel_join:
            handler::chat::join::handle(in, out, user);
            break;
        case io::packet_id::in_beatmap_info_request:break;
        case io::packet_id::in_match_transfer_host:
            handler::multiplayer::room::change_host::handle(in, out, user);
            break;
        case io::packet_id::in_friend_add:
            handler::friends::add::handle(in, out, user);
            break;
        case io::packet_id::in_friend_remove:
            handler::friends::remove::handle(in, out, user);
            break;
        case io::packet_id::in_match_change_team:break;
        case io::packet_id::in_channel_leave:
            handler::chat::leave::handle(in, out, user);
            break;
        case io::packet_id::in_receive_updates:break;
        case io::packet_id::in_set_irc_away_message:break;
        case io::packet_id::in_user_stats_request:
            handler::stats::request_all::handle(in, out, user);
            break;
        case io::packet_id::in_invite:break;
        case io::packet_id::in_match_change_password:
            handler::multiplayer::room::change_password::handle(in, out, user);
            break;
        case io::packet_id::in_special_match_info_request:break;
        case io::packet_id::in_user_presence_request:
            handler::presence::request::handle(in, out, user);
            break;
        case io::packet_id::in_user_presence_request_all:
            handler::presence::request_all::handle(in, out, user);
            break;
        case io::packet_id::in_user_toggle_block_non_friend_pm:break;
        case io::packet_id::in_match_abort:break;
        case io::packet_id::in_special_join_match_channel:break;
        case io::packet_id::in_special_leave_match_channel:break;
        default:
            LOG_F(WARNING, "Packet %i was sent for incoming packet handling while being outbound.", (uint16_t) packet_id);
            return;
    }

    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    user->last_ping = seconds;
}
