/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2020 Marc3842h, czapek
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

#include "../multiplayer/match_manager.hh"
#include "../users/user_manager.hh"
#include "../users/user_punishments.hh"
#include "../utils/bot_utils.hh"
#include "../utils/curler.hh"
#include "../utils/osu_client.hh"
#include "invite_handler.hh"

void shiro::handler::invite::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    if (user->hidden || users::punishments::is_silenced(user->user_id))
        return;

    int32_t user_id = in.data.read<int32_t>();
    std::shared_ptr<users::user> target = users::manager::get_user_by_id(user_id);

    if (target == nullptr || target->hidden)
        return;

    if (target->client_type == +utils::clients::osu_client::aschente) {
        utils::bot::respond("Thanks for the invite but I have to decline :)", user, config::bot::name, true);
        return;
    }

    std::optional<io::layouts::multiplayer_match> optional = shiro::multiplayer::match_manager::get_match(user);

    if (!optional.has_value())
        return;

    io::layouts::multiplayer_match match = *optional;
    std::string url = "osump://" + std::to_string(match.match_id) + "/";
    auto iterator = std::find(match.multi_slot_id.begin(), match.multi_slot_id.end(), user->user_id);

    // If the sending user is in the lobby themselves, we can send the password without a problem
    if (iterator != match.multi_slot_id.end()) {
        if (!match.game_password.empty())
            url.append(utils::curl::escape_url(match.game_password));
    }

    io::osu_writer writer;
    io::layouts::message message;

    message.sender = user->presence.username;
    message.sender_id = user->user_id;

    message.content = "Hey! Come and join my multiplayer room: [" + url + " " + match.game_name + "]";
    message.channel = user->presence.username;

    writer.send_message(message);
    target->queue.enqueue(writer);

    io::osu_writer self_writer;
    message.channel = target->presence.username;

    self_writer.send_message(message);
    user->queue.enqueue(self_writer);
}
