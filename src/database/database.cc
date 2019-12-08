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

#include "../config/cli_args.hh"
#include "../logger/sentry_logger.hh"
#include "../thirdparty/loguru.hh"
#include "tables/beatmap_table.hh"
#include "tables/channel_table.hh"
#include "tables/punishments_table.hh"
#include "tables/relationship_table.hh"
#include "tables/roles_table.hh"
#include "tables/score_table.hh"
#include "tables/user_stats_table.hh"
#include "tables/user_table.hh"
#include "database.hh"

shiro::database::database(const std::string &address, uint32_t port, const std::string &db, const std::string &username, const std::string &password)
    : address(address)
    , port(port)
    , db(db)
    , username(username)
    , password(password) {
    auto [argc, argv] = config::cli::get_args();
    sqlpp::mysql::global_library_init(argc, argv);
}

void shiro::database::connect() {
    if (this->is_connected())
        return;

    this->config = std::make_shared<sqlpp::mysql::connection_config>();
    this->config->host = this->address;
    this->config->port = this->port;
    this->config->database = this->db;
    this->config->user = this->username;
    this->config->password = this->password;
    this->config->auto_reconnect = true;

    #if defined(_DEBUG) && !defined(SHIRO_NO_SQL_DEBUG)
        this->config->debug = true;
    #endif
}

void shiro::database::setup() {
    if (!this->is_connected(true))
        return;

    sqlpp::mysql::connection db(this->config);

    tables::migrations::beatmaps::create(db);
    tables::migrations::channels::create(db);
    tables::migrations::punishments::create(db);
    tables::migrations::relationships::create(db);
    tables::migrations::roles::create(db);
    tables::migrations::scores::create(db);
    tables::migrations::user_stats::create(db);
    tables::migrations::users::create(db);

    LOG_F(INFO, "Successfully connected and structured MySQL database.");
}

bool shiro::database::is_connected(bool abort) {
    if (this->config == nullptr)
        return false;

    try {
        sqlpp::mysql::connection db(this->config);

        return db.is_valid();
    } catch (const sqlpp::exception &ex) {
        logging::sentry::exception(ex);

        if (abort)
            ABORT_F("Unable to connect to database: %s.", ex.what());

        return false;
    }

    return true;
}

std::shared_ptr<sqlpp::mysql::connection_config> shiro::database::get_config() {
    return this->config;
}
