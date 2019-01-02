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

#ifndef SHIRO_SENTRY_LOGGER_HH
#define SHIRO_SENTRY_LOGGER_HH

#include <string>

#include "../thirdparty/crow.hh"
#include "../thirdparty/sentry/crow.hh"
#include "../thirdparty/loguru.hh"

namespace shiro::logging::sentry {

    extern nlohmann::crow client;

    void init();

    void callback(void *user_data, const loguru::Message &message);

    void fatal_callback(const loguru::Message& message);

    void exception(const std::exception &ex);

    void exception(const std::exception_ptr &ptr);

    void http_request_out(const std::string &url, const std::string &method = "GET", int32_t status_code = 200, const std::string &reason = "OK");

    void http_request_in(const ::crow::request &request);

    std::string verbosity_to_sentry_level(const loguru::Verbosity &verbosity);

}

#endif //SHIRO_SENTRY_LOGGER_HH
