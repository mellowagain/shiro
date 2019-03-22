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

#include "../config/ipc_file.hh"
#include "../database/tables/punishments_table.hh"
#include "../thirdparty/loguru.hh"
#include "../utils/bot_utils.hh"
#include "../utils/login_responses.hh"
#include "../utils/punishment_type.hh"
#include "../shiro.hh"
#include "user_manager.hh"
#include "user_punishments.hh"

void shiro::users::punishments::init() {
    scheduler.Schedule(1min, [](tsc::TaskContext ctx) {
        sqlpp::mysql::connection db(db_connection->get_config());
        const tables::punishments punishments_table {};

        auto result = db(select(all_of(punishments_table)).from(punishments_table).where(
                punishments_table.active == true
        ));

        if (result.empty()) {
            ctx.Repeat();
            return;
        }

        std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
        );

        for (const auto &row : result) {
            if (row.duration.is_null())
                continue;

            utils::punishment_type type = (utils::punishment_type) static_cast<uint16_t>(row.type);
            int32_t timestamp = row.time;
            int32_t duration = row.duration;

            if (seconds.count() >= (timestamp + duration)) {
                db(update(punishments_table).set(
                        punishments_table.active = false
                ).where(punishments_table.id == row.id));

                switch (type) {
                    case utils::punishment_type::silence: {
                        LOG_F(INFO, "User %i has been unsilenced automatically.", (int32_t) row.id);
                        break;
                    }
                    case utils::punishment_type::restrict: {
                        std::shared_ptr<user> user = manager::get_user_by_id(row.id);

                        if (user != nullptr) {
                            io::osu_writer writer;

                            writer.announce("Your restriction has ended. Please login again.");
                            writer.login_reply((int32_t) utils::login_responses::account_password_reset);

                            user->queue.enqueue(writer);
                        }

                        LOG_F(INFO, "User %i has been unrestricted automatically.", (int32_t) row.id);
                        break;
                    }
                    case utils::punishment_type::ban: {
                        LOG_F(INFO, "User %i has been unbanned automatically.", (int32_t) row.id);
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }
        }

        ctx.Repeat();
    });
}

void shiro::users::punishments::kick(int32_t user_id, int32_t origin, const std::string &reason) {
    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::punishments punishments_table {};

    db(insert_into(punishments_table).set(
            punishments_table.user_id = user_id,
            punishments_table.origin_id = origin,
            punishments_table.type = (uint16_t) utils::punishment_type::kick,
            punishments_table.time = seconds.count(),
            punishments_table.active = false,
            punishments_table.reason = reason
    ));

    std::shared_ptr<user> user = manager::get_user_by_id(user_id);
    std::string username = manager::get_username_by_id(user_id);
    std::string origin_username = manager::get_username_by_id(origin);

    LOG_F(INFO, "%s has been kicked for %s by %s.", username.c_str(), reason.c_str(), origin_username.c_str());

    if (user == nullptr)
        return;

    io::osu_writer writer;

    writer.announce(reason);
    writer.login_reply((int32_t) utils::login_responses::invalid_credentials);

    user->queue.enqueue(writer);
}

void shiro::users::punishments::silence(int32_t user_id, int32_t origin, uint32_t duration, const std::string &reason) {
    if (is_silenced(user_id))
        return;

    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::punishments punishments_table {};

    db(insert_into(punishments_table).set(
            punishments_table.user_id = user_id,
            punishments_table.origin_id = origin,
            punishments_table.type = (uint16_t) utils::punishment_type::silence,
            punishments_table.time = seconds.count(),
            punishments_table.duration = duration,
            punishments_table.active = true,
            punishments_table.reason = reason
    ));

    std::shared_ptr<user> user = manager::get_user_by_id(user_id);
    std::string username = manager::get_username_by_id(user_id);
    std::string origin_username = manager::get_username_by_id(origin);

    LOG_F(INFO, "%s has been silenced for %i seconds for %s by %s.", username.c_str(), duration, reason.c_str(), origin_username.c_str());

    if (user == nullptr)
        return;

    io::osu_writer global_writer;
    global_writer.user_silenced(user_id);

    users::manager::iterate([user, &global_writer](std::shared_ptr<users::user> online_user) {
        online_user->queue.enqueue(global_writer);
    }, true);

    utils::bot::respond(
            "You have been silenced for " + std::to_string(duration) + " seconds for " + reason + ".",
            user, config::bot::name, true
    );

    io::osu_writer writer;
    writer.user_ban_info(duration); // This will lock the client

    user->queue.enqueue(writer);
}

void shiro::users::punishments::restrict(int32_t user_id, int32_t origin, const std::string &reason) {
    if (is_restricted(user_id))
        return;

    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::punishments punishments_table {};

    db(insert_into(punishments_table).set(
            punishments_table.user_id = user_id,
            punishments_table.origin_id = origin,
            punishments_table.type = (uint16_t) utils::punishment_type::restrict,
            punishments_table.time = seconds.count(),
            punishments_table.active = true,
            punishments_table.reason = reason
    ));

    std::shared_ptr<user> user = manager::get_user_by_id(user_id);
    std::string username = manager::get_username_by_id(user_id);
    std::string origin_username = manager::get_username_by_id(origin);

    LOG_F(INFO, "%s has been restricted for %s by %s.", username.c_str(), reason.c_str(), origin_username.c_str());

    if (user == nullptr)
        return;

    user->hidden = true;

    utils::bot::respond(
            "[" + user->get_url() + " Your account has been restricted]. "
            "Because of that, your profile has been hidden from the public. "
            "If you believe this is a mistake, [" + config::ipc::frontend_url + "support contact support] "
            "to have your account status reviewed.",
            user, config::bot::name, true
    );

    io::layouts::user_quit quit;
    io::osu_writer writer;

    quit.user_id = user_id;
    quit.state = 0;

    writer.user_quit(quit);

    users::manager::iterate([user_id, &writer](std::shared_ptr<users::user> online_user) {
        if (online_user->user_id == user_id)
            return;

        online_user->queue.enqueue(writer);
    });
}

void shiro::users::punishments::ban(int32_t user_id, int32_t origin, const std::string &reason) {
    if (is_banned(user_id))
        return;

    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::punishments punishments_table {};

    db(insert_into(punishments_table).set(
            punishments_table.user_id = user_id,
            punishments_table.origin_id = origin,
            punishments_table.type = (uint16_t) utils::punishment_type::ban,
            punishments_table.time = seconds.count(),
            punishments_table.active = true,
            punishments_table.reason = reason
    ));

    std::shared_ptr<user> user = manager::get_user_by_id(user_id);
    std::string username = manager::get_username_by_id(user_id);
    std::string origin_username = manager::get_username_by_id(origin);

    LOG_F(INFO, "%s has been banned for %s by %s.", username.c_str(), reason.c_str(), origin_username.c_str());

    if (user == nullptr)
        return;

    io::osu_writer writer;

    writer.announce(reason);
    writer.login_reply((int32_t) utils::login_responses::user_banned); // This will lock the client

    user->queue.enqueue(writer);
}

bool shiro::users::punishments::is_silenced(int32_t user_id) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::punishments punishments_table {};

    auto result = db(select(all_of(punishments_table)).from(punishments_table).where(
            punishments_table.user_id == user_id and
            punishments_table.type == (uint16_t) utils::punishment_type::silence and
            punishments_table.active == true
    ).limit(1u));

    return !result.empty();
}

bool shiro::users::punishments::is_restricted(int32_t user_id) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::punishments punishments_table {};

    auto result = db(select(all_of(punishments_table)).from(punishments_table).where(
            punishments_table.user_id == user_id and
            punishments_table.type == (uint16_t) utils::punishment_type::restrict and
            punishments_table.active == true
    ).limit(1u));

    return !result.empty();
}

bool shiro::users::punishments::is_banned(int32_t user_id) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::punishments punishments_table {};

    auto result = db(select(all_of(punishments_table)).from(punishments_table).where(
            punishments_table.user_id == user_id and
            punishments_table.type == (uint16_t) utils::punishment_type::ban and
            punishments_table.active == true
    ).limit(1u));

    return !result.empty();
}

bool shiro::users::punishments::can_chat(int32_t user_id) {
    std::shared_ptr<user> user = manager::get_user_by_id(user_id);

    if (user == nullptr)
        return false;

    if (user->hidden)
        return false;

    return !is_silenced(user_id);
}

bool shiro::users::punishments::has_scores(int32_t user_id) {
    return !is_restricted(user_id) && !is_banned(user_id);
}

std::tuple<int32_t, uint32_t> shiro::users::punishments::get_silence_time(int32_t user_id) {
    if (!is_silenced(user_id))
        return {};

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::punishments punishments_table {};

    auto result = db(select(all_of(punishments_table)).from(punishments_table).where(
            punishments_table.user_id == user_id and
            punishments_table.type == (uint16_t) utils::punishment_type::silence and
            punishments_table.active == true
    ).limit(1u));

    if (result.empty())
        return {};

    const auto &row = result.front();

    return std::make_pair(row.time, row.duration);
}
