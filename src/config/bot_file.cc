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

#include "../logger/sentry_logger.hh"
#include "../thirdparty/cpptoml.hh"
#include "../thirdparty/loguru.hh"
#include "bot_file.hh"
#include "cli_args.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

std::string shiro::config::bot::name = "Shiro";

void shiro::config::bot::parse() {
    if (config_file != nullptr)
        LOG_F(INFO, "Re-parsing bot.toml file...");

    try {
        config_file = cpptoml::parse_file("bot.toml");
    } catch (const cpptoml::parse_exception &ex) {
        logging::sentry::exception(ex);
        ABORT_F("Failed to parse bot.toml file: %s.", ex.what());
    }

    name = config_file->get_qualified_as<std::string>("bot.name").value_or("Shiro");

    LOG_F(INFO, "Successfully parsed bot.toml.");

    cli::cli_app.add_option("--bot-name", name, "Name of the chat bot");
}
