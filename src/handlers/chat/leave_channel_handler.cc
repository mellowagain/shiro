#include "../../channels/channel_manager.hh"
#include "../../users/user_manager.hh"
#include "leave_channel_handler.hh"

void shiro::handler::chat::leave::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    std::string channel = in.data.read_string();
    uint32_t target_channel = channels::manager::get_channel_id(channel);

    if (target_channel == 0)
        return;

    channels::manager::leave_channel(target_channel, user);

    io::osu_writer writer;
    channels::manager::write_channels(writer, user, false);

    for (const std::shared_ptr<users::user> &online_user : users::manager::online_users) {
        online_user->queue.enqueue(writer);
    }

    out.channel_revoked(channel);
}
