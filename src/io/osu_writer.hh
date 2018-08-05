#ifndef SHIRO_OSU_WRITER_HH
#define SHIRO_OSU_WRITER_HH

#include "layouts/channel/channel.hh"
#include "layouts/message/message.hh"
#include "layouts/serializable.hh"
#include "layouts/user/user_presence.hh"
#include "layouts/user/user_quit.hh"
#include "layouts/user/user_stats.hh"
#include "layouts/packets.hh"
#include "osu_buffer.hh"

namespace shiro::io {

    class osu_writer {
    private:
        buffer buf;

        void write(packet_id id) {
            buf.write<int16_t>((int16_t)id);
            buf.write<uint8_t>(0);
            buf.write<int32_t>(0);
        }

        template <typename t = serializable>
        void write(packet_id id, t data) {
            buf.write<int16_t>((int16_t)id);
            buf.write<uint8_t>(0);
            buf.write<int32_t>(data.get_size());
            buf.append(data.marshal().serialize());
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

        void user_presence(layouts::user_presence presence);
        void user_stats(layouts::user_stats stats);

        void friend_list(std::vector<int32_t> friends);
        void users_list(std::vector<int32_t> users);

        void send_message(layouts::message message);

        void rtx(std::string rtx);

        std::string serialize();
        buffer &get_buffer();

    };

}

#endif //SHIRO_OSU_WRITER_HH
