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
#include "bancho_file.hh"
#include "cli_args.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

std::string shiro::config::bancho::host = "127.0.0.1";
uint16_t shiro::config::bancho::port = 8080;
uint16_t shiro::config::bancho::concurrency = 64;

bool shiro::config::bancho::default_supporter = false;

std::string shiro::config::bancho::api_key = "osu! API key";

std::string shiro::config::bancho::alert = "Welcome to Shiro";
std::string shiro::config::bancho::title_image = "https://i.ppy.sh/motd.png";
std::string shiro::config::bancho::title_url = "https://shiro.host";

bool shiro::config::bancho::sentry_integration = true;

void shiro::config::bancho::parse() {
    if (config_file != nullptr)
        LOG_F(INFO, "Re-parsing bancho.toml file...");

    try {
        config_file = cpptoml::parse_file("bancho.toml");
    } catch (const cpptoml::parse_exception &ex) {
        logging::sentry::exception(ex);
        ABORT_F("Failed to parse bancho.toml file: %s.", ex.what());
    }

    host = config_file->get_qualified_as<std::string>("server.host").value_or("127.0.0.1");
    port = config_file->get_qualified_as<uint16_t>("server.port").value_or(8080);
    concurrency = config_file->get_qualified_as<uint16_t>("server.concurrency").value_or(64);

    default_supporter = config_file->get_qualified_as<bool>("permissions.default_supporter").value_or(false);

    api_key = config_file->get_qualified_as<std::string>("api.key").value_or("osu! API key");

    alert = config_file->get_qualified_as<std::string>("motd.alert").value_or("Welcome to Shiro");
    title_image = config_file->get_qualified_as<std::string>("motd.title_image").value_or("https://i.ppy.sh/motd.png");
    title_url = config_file->get_qualified_as<std::string>("motd.title_url").value_or("https://shiro.host");

    sentry_integration = config_file->get_qualified_as<bool>("integrations.sentry").value_or(true);

    LOG_F(INFO, "Successfully parsed bancho.toml.");

    cli::cli_app.add_option("--bancho-host", host, "Host that Bancho should bind to");
    cli::cli_app.add_option("--bancho-port", port, "Port that Bancho should bind to");
    cli::cli_app.add_option("--bancho-concurrency", concurrency, "Amount of concurrent connections that should be handled");

    // Boolean flags are not yet supported fully in cli, see score_submission_file.cc
    //cli::cli_app.add_option("--bancho-default-supporter", default_Supporter, "Allow users with no roles / permissions to have supporer in-game?");

    cli::cli_app.add_option("--bancho-api-key", api_key, "API key for accessing official osu!Bancho API");

    cli::cli_app.add_option("--bancho-motd-alert", alert, "Alert to be displayed on login");
    cli::cli_app.add_option("--bancho-motd-title-image", title_image, "Title image to be displayed in osu! main menu");
    cli::cli_app.add_option("--bancho-motd-title-url", title_url, "Title url which opens when clicking on title image");

    // Boolean flags are not yet supported fully in cli, see above
    //cli::cli_app.add_option("--bancho-sentry-integration", sentry_integration, "Integrate Shiro with Sentry.io error reporting?");
}
