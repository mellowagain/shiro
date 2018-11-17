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

#include "../thirdparty/cpptoml.hh"
#include "../thirdparty/loguru.hh"
#include "api_file.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

bool shiro::config::api::deploy_enabled = false;
std::vector<std::string> shiro::config::api::deploy_keys {};

void shiro::config::api::parse() {
    if (config_file != nullptr)
        LOG_S(INFO) << "Re-parsing api.toml file...";

    try {
        config_file = cpptoml::parse_file("api.toml");
    } catch (const cpptoml::parse_exception &ex) {
        LOG_S(FATAL) << "Failed to parse api.toml file: " << ex.what() << ".";
    }

    deploy_enabled = config_file->get_qualified_as<bool>("deploy.enabled").value_or(false);
    deploy_keys = config_file->get_array_of<std::string>("deploy.keys").value_or({});

    // Passing arrays and booleans is not fully supported in CLI.
    // Thus, these options can't be configured like that.
    // Needs fixing in the future.

    if (!deploy_keys.empty() || !deploy_enabled)
        return;

    deploy_enabled = false;
    LOG_F(WARNING, "Deployment has been activated but no deploy keys have been specified. Disabling deployment.");
}
