#include "../../handlers/ping_handler.hh"
#include "../../handlers/user_status_handler.hh"
#include "../../thirdparty/loguru.hh"
#include "packet_router.hh"

void shiro::routes::packets::route(shiro::io::packet_id packet_id, shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    switch (packet_id) {
        case io::packet_id::in_send_user_status:
            handler::user_status::handle(in, out, user);
            break;
        case io::packet_id::in_send_irc_message:break;
        case io::packet_id::in_exit:break;
        case io::packet_id::in_request_status_update:break;
        case io::packet_id::in_pong:
            handler::ping::handle(in, out, user);
            break;
        case io::packet_id::in_start_spectating:break;
        case io::packet_id::in_stop_spectating:break;
        case io::packet_id::in_spectate_frames:break;
        case io::packet_id::in_error_report:break;
        case io::packet_id::in_cant_spectate:break;
        case io::packet_id::in_send_irc_message_private:break;
        case io::packet_id::in_lobby_part:break;
        case io::packet_id::in_lobby_join:break;
        case io::packet_id::in_match_create:break;
        case io::packet_id::in_match_join:break;
        case io::packet_id::in_match_part:break;
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
        case io::packet_id::in_channel_join:break;
        case io::packet_id::in_beatmap_info_request:break;
        case io::packet_id::in_match_transfer_host:break;
        case io::packet_id::in_friend_add:break;
        case io::packet_id::in_friend_remove:break;
        case io::packet_id::in_match_change_team:break;
        case io::packet_id::in_channel_leave:break;
        case io::packet_id::in_receive_updates:break;
        case io::packet_id::in_set_irc_away_message:break;
        case io::packet_id::in_user_stats_request:break;
        case io::packet_id::in_invite:break;
        case io::packet_id::in_match_change_password:break;
        case io::packet_id::in_special_match_info_request:break;
        case io::packet_id::in_user_presence_request:break;
        case io::packet_id::in_user_presence_request_all:break;
        case io::packet_id::in_user_toggle_block_non_friend_pm:break;
        case io::packet_id::in_match_abort:break;
        case io::packet_id::in_special_join_match_channel:break;
        case io::packet_id::in_special_leave_match_channel:break;
        default:
            LOG_F(WARNING, "Packet %i was sent for incoming packet handling while being outbound.", (uint16_t) packet_id);
            break;
    }
}
