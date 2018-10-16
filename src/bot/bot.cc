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

#include <functional>
#include <unordered_map>
#include <utility>
#include <memory>

#include "../config/bot_file.hh"
#include "../config/db_file.hh"
#include "../database/tables/user_table.hh"
#include "../geoloc/country_ids.hh"
#include "../shiro.hh"
#include "../thirdparty/digestpp.hh"
#include "../thirdparty/loguru.hh"
#include "../thirdparty/uuid.hh"
#include "../users/user_manager.hh"
#include "../utils/escaper.hh"
#include "commands/commands.hh"
#include "bot.hh"

static std::unordered_map<std::string, std::function<bool(std::deque<std::string>&, std::shared_ptr<shiro::users::user>, std::string)>> commands_map;

void shiro::bot::init() {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table{};

    auto result = db(select(all_of(user_table)).from(user_table).where(user_table.id == 1));
    bool empty = is_query_empty(result);

    if (empty) {
        // Bot user doesn't exist
        db(insert_into(user_table).set(
                user_table.id = 1,
                user_table.username = config::bot::name,
                user_table.safe_username = utils::escaper::make_safe(config::bot::name),
                user_table.password = digestpp::sha256().absorb(config::database::password).hexdigest(),
                user_table.salt = config::database::database,
                user_table.email = config::bot::name + "@shiro.host",
                user_table.ip = "127.0.0.1",
                user_table.registration_date = 0,
                user_table.last_seen = 0,
                user_table.followers = 0,
                user_table.groups = 0,
                user_table.user_page = "beep boop",
                user_table.pp_std = 0,
                user_table.pp_taiko = 0,
                user_table.pp_ctb = 0,
                user_table.pp_mania = 0,
                user_table.score_std = 0,
                user_table.score_taiko = 0,
                user_table.score_ctb = 0,
                user_table.score_mania = 0,
                user_table.ranked_score_std = 0,
                user_table.ranked_score_taiko = 0,
                user_table.ranked_score_ctb = 0,
                user_table.ranked_score_mania = 0,
                user_table.play_count_std = 0,
                user_table.play_count_taiko = 0,
                user_table.play_count_ctb = 0,
                user_table.play_count_mania = 0,
                user_table.country = "JP"
        ));
    }

    std::shared_ptr<users::user> bot_user = std::make_shared<users::user>(1);

    if (!bot_user->init()) {
        LOG_S(FATAL) << "Unable to initialize chat bot.";
        return;
    }

    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    bot_user->token = sole::uuid4().str();
    bot_user->client_version = "b19700101.01";
    bot_user->hwid = digestpp::sha256().absorb(config::bot::name).hexdigest();
    bot_user->last_ping = seconds;

    bot_user->presence.country_id = (uint8_t) geoloc::country_id::JP;
    bot_user->presence.time_zone = 9;
    bot_user->presence.latitude = 35.6895f;
    bot_user->presence.longitude = 139.6917f;

    users::manager::login_user(bot_user);

    scheduler.Schedule(30s, [bot_user](tsc::TaskContext ctx) {
        bot_user->queue.clear();

        ctx.Repeat(30s);
    });

    LOG_S(INFO) << "Bot has been successfully registered as " << config::bot::name << " and is now online.";
}

void shiro::bot::init_commands() {
    commands_map.insert(std::make_pair("announce", commands::announce));
    commands_map.insert(std::make_pair("clear", commands::clear));
    commands_map.insert(std::make_pair("clients", commands::clients));
    commands_map.insert(std::make_pair("help", commands::help));
    commands_map.insert(std::make_pair("pp", commands::pp));
    commands_map.insert(std::make_pair("rank", commands::rank));
    commands_map.insert(std::make_pair("roll", commands::roll));
    commands_map.insert(std::make_pair("rtx", commands::rtx));

    LOG_S(INFO) << "Bot commands have been successfully loaded. " << commands_map.size() << " commands available.";
}

bool shiro::bot::handle(const std::string &command, std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    try {
        return commands_map.at(command)(args, user, channel);
    } catch (const std::out_of_range &ex) {
        io::layouts::message msg;
        io::osu_writer writer;

        msg.sender = config::bot::name;
        msg.sender_id = 1;
        msg.channel = channel;
        msg.content = "!" + command + " could not be found. Type !help to get a list of available commands.";

        writer.send_message(msg);
        user->queue.enqueue(writer);
    }

    return false;
}
