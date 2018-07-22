#include <functional>
#include <unordered_map>
#include <utility>

#include "../config/bot_file.hh"
#include "../config/db_file.hh"
#include "../geoloc/country_ids.hh"
#include "../shiro.hh"
#include "../thirdparty/digestpp.hh"
#include "../thirdparty/loguru.hh"
#include "../thirdparty/uuid.hh"
#include "../users/user_manager.hh"
#include "commands/rtx_command.hh"
#include "bot.hh"

static std::unordered_map<std::string, std::function<bool(std::deque<std::string>&, std::shared_ptr<shiro::users::user>)>> commands_map;

void shiro::bot::init() {
    using exists_struct = std::tuple<bool>;

    // Bot is always id 1
    std::vector<exists_struct> exists_result = db_connection->query<exists_struct>("SELECT EXISTS(SELECT 1 FROM `users` WHERE id = 1);");

    if (exists_result.empty() || !std::get<0>(exists_result.at(0))) {
        db_connection->update("INSERT INTO `users` (id, "
                              "username, safe_username, "
                              "password, salt, "
                              "email, ip, "
                              "registration_date, last_seen, "
                              "followers, groups, user_page, "
                              "pp_std, pp_taiko, pp_ctb, pp_mania,"
                              "score_std, score_taiko, score_ctb, score_mania,"
                              "ranked_score_std, ranked_score_taiko, ranked_score_ctb, ranked_score_mania,"
                              "play_count_std, play_count_taiko, play_count_ctb, play_count_mania,"
                              "country) "
                              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
                              1,
                              config::bot::name, config::bot::name,
                              digestpp::sha256().absorb(config::database::password).hexdigest(), config::database::database,
                              config::bot::name + "@shiro.host", "127.0.0.1",
                              0, 0,
                              0, 5, "beep boop",
                              0, 0, 0, 0,
                              0, 0, 0, 0,
                              0, 0, 0, 0,
                              0, 0, 0, 0,
                              "JP");
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

    LOG_S(INFO) << "Bot has been successfully registered as " << config::bot::name << " and is now online.";
}

void shiro::bot::init_commands() {
    commands_map.insert(std::make_pair("rtx", commands::rtx));

    LOG_S(INFO) << "Bot commands have been successfully loaded. " << commands_map.size() << " commands available.";
}

bool shiro::bot::handle(const std::string &command, std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user) {
    try {
        return commands_map.at(command)(args, std::move(user));
    } catch (const std::out_of_range &ex) {
        // Command has not been found
        // TODO? Maybe send message to user or smth?
    }

    return false;
}
