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

// Loguro settings
#if defined(_DEBUG)
    #define LOGURU_DEBUG_LOGGING 1
    #define LOGURU_DEBUG_CHECKS 1
#else
    #undef LOGURU_DEBUG_LOGGING
    #undef LOGURU_DEBUG_CHECKS
#endif

#define LOGURU_IMPLEMENTATION 1

#include "../thirdparty/loguru.hh"
#include "logger.hh"

void shiro::logging::init(int argc, char **argv) {
    loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
    loguru::g_flush_interval_ms = 100;

    std::string current_time = []() -> std::string {
        struct std::tm time_struct;
        char buffer[80];

        std::time_t now = std::time(nullptr);
        time_struct = *std::localtime(&now);

        std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H.%M.%S", &time_struct);

        return std::string(buffer);
    }();
    char buffer[128];
    std::snprintf(buffer, sizeof(buffer), "logs/shiro %s.log", current_time.c_str());
    loguru::add_file(buffer, loguru::FileMode::Append, loguru::Verbosity_INFO);

    loguru::init(argc, argv);

    LOG_F(INFO, "Logging was successfully initialized.");
}
