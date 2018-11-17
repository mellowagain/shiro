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
#include "curl_file.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

std::string shiro::config::curl::user_agent = "";
bool shiro::config::curl::cert_verify = true;

void shiro::config::curl::parse() {
    if (config_file != nullptr)
        LOG_S(INFO) << "Re-parsing curl.toml file...";

    try {
        config_file = cpptoml::parse_file("curl.toml");
    } catch (const cpptoml::parse_exception &ex) {
        LOG_S(FATAL) << "Failed to parse curl.toml file: " << ex.what() << ".";
    }

    user_agent = config_file->get_qualified_as<std::string>("curl.user_agent").value_or("Mozilla/5.0 (X11; Linux x86_64; rv:61.0) Gecko/20100101 Firefox/61.0");
    cert_verify = config_file->get_qualified_as<bool>("curl.cert_verify").value_or(true);
}
