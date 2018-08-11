#include <boost/algorithm/string.hpp>
#include <deque>
#include <vector>

#include "../../channels/channel_manager.hh"
#include "../../io/layouts/message/message.hh"
#include "../../thirdparty/loguru.hh"
#include "../../users/user_manager.hh"
#include "../../utils/bot_utils.hh"
#include "../../utils/string_utils.hh"
#include "private_chat_handler.hh"

void shiro::handler::chat::handle_private(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    io::osu_writer message_buffer;
    io::layouts::message message = in.unmarshal<io::layouts::message>();

    std::shared_ptr<users::user> target_user = users::manager::get_user_by_username(message.channel);

    if (target_user == nullptr)
        return;

    // Bot user
    if (target_user->user_id == 1) {
        utils::bot::handle(message, user);
        return;
    }

    message.sender = user->presence.username;
    message.sender_id = user->user_id;
    message.channel = user->presence.username;

    message_buffer.send_message(message);
    target_user->queue.enqueue(message_buffer);
}
