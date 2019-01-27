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

#include "../thirdparty/loguru.hh"
#include "cli_args.hh"

static int args_count = 0;
static char **args_vector {};

CLI::App shiro::config::cli::cli_app = CLI::App("C++ osu!Bancho re-implementation", "shiro");

void shiro::config::cli::parse(int argc, char **argv) {
    args_count = argc;
    args_vector = argv;

    try {
        cli_app.parse(argc, argv);
    } catch (const CLI::ParseError &ex) {
        LOG_S(FATAL) << "Unable to parse command line arguments: " << ex.what() << ".";
    }
}

std::tuple<int, char**> shiro::config::cli::get_args() {
    return std::make_tuple(args_count, args_vector);
}
