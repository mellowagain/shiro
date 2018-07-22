#include <boost/algorithm/string.hpp>
#include <deque>
#include <vector>

#include "../../bot/bot.hh"
#include "../../channels/channel_manager.hh"
#include "../../io/layouts/message/message.hh"
#include "../../thirdparty/loguru.hh"
#include "../../utils/string_utils.hh"
#include "public_chat_handler.hh"

void shiro::handler::chat::handle_public(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    io::osu_writer message_buffer;
    io::layouts::message message = in.unmarshal<io::layouts::message>();

    message.sender = user->presence.username;
    message.sender_id = user->user_id;

    if (boost::algorithm::starts_with(message.content, "!")) {
        std::vector<std::string> splitted = utils::strings::split(message.content.substr(1), ' ');

        if (splitted.empty())
            return;

        std::string command = splitted.at(0);
        std::deque args(splitted.begin(), splitted.end());
        args.pop_front(); // Remove command which is the first argument

        bot::handle(command, args, user);
    }

    message_buffer.send_message(message);

    auto users = channels::manager::get_users_in_channel(message.channel);

    for (const std::shared_ptr<users::user> &channel_user : users) {
        if (user->user_id == channel_user->user_id || user->user_id == 1)
            continue;

        channel_user->queue.enqueue(message_buffer);
    }
}
