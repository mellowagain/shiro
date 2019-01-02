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

#include "../logger/sentry_logger.hh"
#include "../thirdparty/cpptoml.hh"
#include "../thirdparty/loguru.hh"
#include "cli_args.hh"
#include "db_file.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

std::string shiro::config::database::address = "127.0.0.1";
uint32_t shiro::config::database::port = 3306;
std::string shiro::config::database::database = "shiro";
std::string shiro::config::database::username = "root";
std::string shiro::config::database::password = "hunter2";

void shiro::config::database::parse() {
    if (config_file != nullptr)
        LOG_S(INFO) << "Re-parsing database.toml file...";

    try {
        config_file = cpptoml::parse_file("database.toml");
    } catch (const cpptoml::parse_exception &ex) {
        logging::sentry::exception(ex);
        LOG_S(FATAL) << "Failed to parse database.toml file: " << ex.what() << ".";
    }

    address = config_file->get_qualified_as<std::string>("database.address").value_or("127.0.0.1");
    port = config_file->get_qualified_as<uint32_t>("database.port").value_or(3306);
    database = config_file->get_qualified_as<std::string>("database.database").value_or("shiro");
    username = config_file->get_qualified_as<std::string>("database.username").value_or("root");
    password = config_file->get_qualified_as<std::string>("database.password").value_or("hunter2");

    LOG_S(INFO) << "Successfully parsed database.toml.";

    cli::cli_app.add_option("--db-address", address, "Address of MySQL server to connect to");
    cli::cli_app.add_option("--db-port", port, "Port of MySQL server to connect to");
    cli::cli_app.add_option("--db-database", database, "Database in MySQL server to put data into");
    cli::cli_app.add_option("--db-username", username, "Username used to authenticate with MySQL server");
    cli::cli_app.add_option("--db-password", password, "Password used to authenticate with MySQL server");
}
