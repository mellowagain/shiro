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

#include "../config/bancho_file.hh"
#include "sentry_logger.hh"

nlohmann::crow shiro::logging::sentry::client("https://da055b9caaf34778a93db094a1f8b6c2@sentry.io/1277337");

void shiro::logging::sentry::init() {
    if (!config::bancho::sentry_integration) {
        LOG_F(WARNING, "Sentry.io error reporting is currently disabled.");
        return;
    }

    loguru::add_callback("sentry.io", logging::sentry::callback, nullptr, loguru::Verbosity_INFO);
    loguru::set_fatal_handler(logging::sentry::fatal_callback);
    client.add_breadcrumb("Successfully started communication with sentry.io.");
}

void shiro::logging::sentry::callback(void *user_data, const loguru::Message &message) {
    switch (message.verbosity) {
        case loguru::Verbosity_ERROR:
        case loguru::Verbosity_FATAL:
            client.capture_message(message.message, {
                    { "level", verbosity_to_sentry_level(message.verbosity) },
                    { "extra",
                        {
                            { "verbosity", message.verbosity },
                            { "filename", message.filename },
                            { "line", message.line },
                            { "prefix", message.prefix }
                        }
                    }
            });
            break;
        default:
            client.add_breadcrumb(message.message, {
                    { "level", verbosity_to_sentry_level(message.verbosity) },
                    { "data",
                        {
                            { "verbosity", message.verbosity },
                            { "filename", message.filename },
                            { "line", message.line },
                            { "prefix", message.prefix }
                        }
                    }
            });
            break;
    }
}

void shiro::logging::sentry::fatal_callback(const loguru::Message &message) {
    client.add_breadcrumb("!! FATAL ERROR OCCURRED, EXITING NOW !!");
    client.capture_message(message.message, {
            { "level", "fatal" },
            { "extra",
                {
                    { "verbosity", message.verbosity },
                    { "filename", message.filename },
                    { "line", message.line },
                    { "prefix", message.prefix }
                }
            }
    });
}

void shiro::logging::sentry::exception(const std::exception &ex) {
    client.capture_exception(ex);
}

void shiro::logging::sentry::exception(const std::exception_ptr &ptr) {
    if (!config::bancho::sentry_integration)
        return;

    try {
        std::rethrow_exception(ptr);
    } catch (const std::exception &ex) {
        client.capture_exception(ex, nullptr, false);
    }
}

void shiro::logging::sentry::http_request_out(const std::string &url, const std::string &method, int32_t status_code, const std::string &reason) {
    if (!config::bancho::sentry_integration)
        return;

    json req = {
        { "url", url },
        { "method", method },
        { "status_code", status_code }
    };

    if (!reason.empty())
        req["reason"] = reason;

    client.add_breadcrumb("", {
            { "level", "info" },
            { "type", "http" },
            { "data", req }
    });
}

void shiro::logging::sentry::http_request_in(const ::crow::request &request) {
    if (!config::bancho::sentry_integration)
        return;

    json req = {
        { "url", request.url },
        { "method", ::crow::method_name(request.method) },
        { "data", request.body },
        { "query_string", request.raw_url },
        { "env",
            {
                "REMOTE_ADDR", request.get_ip_address()
            }
        }
    };

    json headers;

    for (const auto &[key, value] : request.headers) {
        headers[key] = value;
    }

    req["headers"] = headers;

    std::string cookie = request.get_header_value("cookie");

    if (!cookie.empty())
        req["cookies"] = cookie;

    client.add_request_context(req);
}

std::string shiro::logging::sentry::verbosity_to_sentry_level(const loguru::Verbosity &verbosity) {
    switch (verbosity) {
        case loguru::Verbosity_ERROR:
            return "error";
        case loguru::Verbosity_FATAL:
            return "fatal";
        case loguru::Verbosity_WARNING:
            return "warning";
        case loguru::Verbosity_INFO:
            return "info";
        default:
            return "debug";
    }
}
