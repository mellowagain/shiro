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

#ifndef SHIRO_OSU_WRITER_HH
#define SHIRO_OSU_WRITER_HH

#include "layouts/channel/channel.hh"
#include "layouts/message/message.hh"
#include "layouts/replay/spectate_frames.hh"
#include "layouts/user/user_presence.hh"
#include "layouts/user/user_quit.hh"
#include "layouts/user/user_stats.hh"
#include "layouts/packets.hh"
#include "layouts/serializable.hh"
#include "osu_buffer.hh"

namespace shiro::io {

    class osu_writer {
    private:
        buffer buf;

        void write(packet_id id) {
            buf.write<int16_t>((int16_t) id);
            buf.write<uint8_t>(0);
            buf.write<int32_t>(0);
        }

        template <typename t = serializable>
        void write(packet_id id, t data) {
            buf.write<int16_t>((int16_t) id);
            buf.write<uint8_t>(0);
            buf.write<int32_t>(data.get_size());
            buf.append(data.marshal().serialize());
        }

        void write(packet_id id, buffer &buf) {
            buf.write<int16_t>((int16_t) id);
            buf.write<uint8_t>(0);
            buf.write<int32_t>(buf.get_size());
            buf.append(buf);
        }

    public:
        void login_reply(int32_t reply);
        void login_permissions(int32_t permissions);

        void user_quit(layouts::user_quit user_quit);

        void announce(std::string announcement);

        void protocol_negotiation(int32_t protocol_version);

        void channel_listing_complete();
        void channel_available(layouts::channel channel);
        void channel_join(std::string channel_name);
        void channel_revoked(std::string channel_name);

        void user_presence(layouts::user_presence presence);
        void user_stats(layouts::user_stats stats);
        void user_silenced(int32_t id);
        void user_ban_info(int32_t duration);

        void friend_list(std::vector<int32_t> friends);
        void users_list(std::vector<int32_t> users);

        void send_message(layouts::message message);

        void rtx(std::string rtx);

        void spectator_join_user(int32_t id);
        void spectator_join_host(int32_t id);
        void spectator_left_user(int32_t id);
        void spectator_left_host(int32_t id);
        void spectator_cant_spectate(int32_t id);
        void spectate_frames(layouts::spectate_frames frames);

        std::string serialize();
        buffer &get_buffer();

    };

}

#endif //SHIRO_OSU_WRITER_HH
