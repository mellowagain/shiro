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

#include <boost/algorithm/string.hpp>
#include <deque>
#include <vector>

#include "../../channels/channel_manager.hh"
#include "../../io/layouts/message/message.hh"
#include "../../thirdparty/loguru.hh"
#include "../../users/user_manager.hh"
#include "../../users/user_punishments.hh"
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

    if (!users::punishments::can_chat(user->user_id)) {
        utils::bot::respond("You are unable to chat while being restricted.", user, message.channel, true);
        return;
    }

    if (!users::punishments::can_chat(target_user->user_id))
        utils::bot::respond(
                "The user you tried to chat is currently unable to reply because of a chat restriction.",
                user,
                message.channel,
                true
        );

    message.sender = user->presence.username;
    message.sender_id = user->user_id;
    message.channel = user->presence.username;

    message_buffer.send_message(message);
    target_user->queue.enqueue(message_buffer);
}
