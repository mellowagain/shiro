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

#ifndef SHIRO_COMMAND_MANAGER_HH
#define SHIRO_COMMAND_MANAGER_HH

#include <vector>

#include "../utils/filesystem.hh"
#include "../utils/python_utils.hh"
#include "../users/user.hh"
#include "python_command.hh"

namespace shiro::commands::manager {

    extern std::shared_ptr<utils::python::wrapper> wrapper;
    extern std::vector<python_command> commands;

    void init();

    void find_command_files(const fs::path &path);

    bool execute(const std::string &command, std::deque<std::string> &args, std::shared_ptr<users::user> user, std::string channel);

}

#endif //SHIRO_COMMAND_MANAGER_HH
