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

#if defined(_WIN32)

#include <cstdio>
#include <windows.h>

#include "../../thirdparty/loguru.hh"
#include "../signal_handler.hh"

bool handle_signal(unsigned int signal) {
    switch (signal) {
        case CTRL_C_EVENT:
            LOG_F(INFO, "Shutting down...");
            exit(EXIT_SUCCESS);
        case CTRL_CLOSE_EVENT:
            return true;
        default:
            return false;
    }
}

void shiro::native::signal_handler::install() {
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE) handle_signal, true))
        ABORT_F("Unable to install signal handler.");

    LOG_F(INFO, "Signal handler was successfully installed.");
}

#endif
