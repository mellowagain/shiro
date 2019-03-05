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

#if defined(__linux__) || defined(__APPLE__) // This is a exception because the signal handlers are the same on *nix

#include <csignal>
#include <cstdlib>

#include "../../thirdparty/loguru.hh"
#include "../signal_handler.hh"

void handle_signal(int signal) {
    if (signal != SIGINT)
        return;

    LOG_F(INFO, "Shutting down...");
    std::exit(EXIT_SUCCESS);
}

void shiro::native::signal_handler::install() {
    struct sigaction sig_int_handler;
    sig_int_handler.sa_handler = handle_signal;
    sigemptyset(&sig_int_handler.sa_mask);
    sig_int_handler.sa_flags = 0;

    sigaction(SIGINT, &sig_int_handler, nullptr);

    LOG_F(INFO, "Signal handler was successfully installed.");
}

#endif
