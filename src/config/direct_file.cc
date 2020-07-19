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

#include <memory>

#include "../logger/sentry_logger.hh"
#include "../thirdparty/cpptoml.hh"
#include "../thirdparty/loguru.hh"
#include "direct_file.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

bool shiro::config::direct::enabled = true;
int32_t shiro::config::direct::provider = 1;

// Provider 0: Shirogane
uint64_t shiro::config::direct::shm = 0x0;

// Provider 1: Emulation
std::string shiro::config::direct::base_url = "https://shiro.host";
std::string shiro::config::direct::mirror_url = "https://mirror.shiro.host";

// Provider 2: Beatconnect
std::string shiro::config::direct::api_key;

void shiro::config::direct::parse() {
    if (config_file != nullptr)
        LOG_F(INFO, "Re-parsing direct.toml file...");

    try {
        config_file = cpptoml::parse_file("direct.toml");
    } catch (const cpptoml::parse_exception &ex) {
        logging::sentry::exception(ex);
        ABORT_F("Failed to parse direct.toml file: %s.", ex.what());
    }

    enabled = config_file->get_qualified_as<bool>("direct.enabled").value_or(true);

    if (!enabled)
        return;

    provider = config_file->get_qualified_as<int32_t>("direct.provider").value_or(1);

    switch (provider) {
        case 0:
            shm = config_file->get_qualified_as<uint64_t>("shirogane.shm").value_or(0x0);
            break;
        case 1:
            base_url = config_file->get_qualified_as<std::string>("emulate.base_url").value_or("https://shiro.host");
            mirror_url = config_file->get_qualified_as<std::string>("emulate.mirror_url").value_or("https://mirror.shiro.host");
            break;
        case 2:
            api_key = config_file->get_qualified_as<std::string>("beatconnect.api_key").value_or("");
            break;
        default:
            ABORT_F("Invalid direct mode provided in direct.toml: %i", provider);
    }

    LOG_F(INFO, "Successfully parsed direct.toml.");

    // CLI is currently not supported as these values are related to each other.
}
