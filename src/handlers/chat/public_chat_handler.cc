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
#include "../../spectating/spectator_manager.hh"
#include "../../thirdparty/loguru.hh"
#include "../../users/user_punishments.hh"
#include "../../utils/bot_utils.hh"
#include "../../utils/string_utils.hh"
#include "public_chat_handler.hh"

void shiro::handler::chat::handle_public(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    io::osu_writer message_buffer;
    io::layouts::message message = in.unmarshal<io::layouts::message>();

    if (!users::punishments::can_chat(user->user_id)) {
        utils::bot::respond("You are unable to chat while being restricted.", user, message.channel, true);
        return;
    }

    message.sender = user->presence.username;
    message.sender_id = user->user_id;

    message_buffer.send_message(message);

    if (channels::manager::is_read_only(message.channel))
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

    std::vector<std::shared_ptr<users::user>> users = channels::manager::get_users_in_channel(message.channel);

    for (const std::shared_ptr<users::user> &channel_user : users) {
        if (user->user_id == channel_user->user_id || user->user_id == 1)
            continue;

        channel_user->queue.enqueue(message_buffer);
    }

    utils::bot::handle(message, user);
}
