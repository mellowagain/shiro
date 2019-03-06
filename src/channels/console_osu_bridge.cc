/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2019 Marc3842h, czapek
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

#include "../bot/bot.hh"
#include "../users/user_manager.hh"
#include "../utils/bot_utils.hh"
#include "../utils/osu_permissions.hh"
#include "channel_manager.hh"
#include "console_osu_bridge.hh"

void shiro::channels::bridge::install() {
    loguru::add_callback("osu! bridge", channels::bridge::callback, nullptr, loguru::Verbosity_INFO);
}

void shiro::channels::bridge::callback(void *user_data, const loguru::Message &log_message) {
    std::shared_ptr<users::user> bot_user = bot::bot_user;

    if (bot_user == nullptr)
        return;

    io::osu_writer writer;
    io::osu_writer future_writer;

    io::layouts::message ingame_message;
    ingame_message.sender = config::bot::name;
    ingame_message.sender_id = 1;
    ingame_message.channel = "#console";
    ingame_message.content = log_message.message;

    io::layouts::user_presence bot_presence = bot_user->presence;
    bot_presence.permissions = get_permission(log_message.verbosity);

    writer.user_presence(bot_presence);
    writer.send_message(ingame_message);

    future_writer.user_presence(bot_user->presence);

    for (const std::shared_ptr<users::user> &user : channels::manager::get_users_in_channel("#console")) {
        if (user == nullptr || user->user_id == 1)
            continue;

        user->queue.enqueue(writer);
        user->queue.enqueue_next(future_writer);
    }
}

uint8_t shiro::channels::bridge::get_permission(loguru::Verbosity verbosity) {
    switch (verbosity) {
        case loguru::Verbosity_WARNING:
            return (uint8_t) utils::osu_permissions::supporter;
        case loguru::Verbosity_ERROR:
            return (uint8_t) utils::osu_permissions::bat;
        case loguru::Verbosity_FATAL:
            return (uint8_t) utils::osu_permissions::peppy;
        default:
            return (uint8_t) utils::osu_permissions::friend_;
    }
}
