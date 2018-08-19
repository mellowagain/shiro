#include <boost/algorithm/string.hpp>
#include <deque>
#include <vector>

#include "../../channels/channel_manager.hh"
#include "../../io/layouts/message/message.hh"
#include "../../spectating/spectator_manager.hh"
#include "../../thirdparty/loguru.hh"
#include "../../utils/bot_utils.hh"
#include "../../utils/string_utils.hh"
#include "public_chat_handler.hh"

void shiro::handler::chat::handle_public(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    io::osu_writer message_buffer;
    io::layouts::message message = in.unmarshal<io::layouts::message>();

    message.sender = user->presence.username;
    message.sender_id = user->user_id;

    message_buffer.send_message(message);

    if (message.channel == "#announce")
        return;

    if (message.channel == "#spectator") {
        std::vector<std::shared_ptr<users::user>> spectators = spectating::manager::get_spectators(user);
        std::shared_ptr<users::user> host = spectating::manager::get_host(user);

        for (const std::shared_ptr<users::user> &spectator : spectators) {
            if (spectator == user)
                continue;

            spectator->queue.enqueue(message_buffer);
        }

        if (host != nullptr)
            host->queue.enqueue(message_buffer);

        utils::bot::handle(message, user);
        return;
    }

    auto users = channels::manager::get_users_in_channel(message.channel);

    for (const std::shared_ptr<users::user> &channel_user : users) {
        if (user->user_id == channel_user->user_id || user->user_id == 1)
            continue;

        channel_user->queue.enqueue(message_buffer);
    }

    utils::bot::handle(message, user);
}
