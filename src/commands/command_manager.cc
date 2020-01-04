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

#include <python3.8/Python.h>
#include <boost/algorithm/string.hpp>

#include "../thirdparty/loguru.hh"
#include "../native/process_info.hh"
#include "../utils/python_utils.hh"
#include "command_manager.hh"

std::shared_ptr<shiro::utils::python::wrapper> shiro::commands::manager::wrapper = nullptr;
std::vector<shiro::commands::python_command> shiro::commands::manager::commands;

void shiro::commands::manager::init() {
    wrapper = std::make_shared<utils::python::wrapper>();

    fs::path commands_path = fs::current_path() / "commands";

    // Make Python load commands from the commands directory
    native::process_info::set_env("PYTHONPATH", commands_path.u8string());
    Py_Initialize();

    utils::python::init();

    find_command_files(commands_path);

    LOG_F(INFO, "Python bot commands have been successfully loaded. %lu commands available.", commands.size());
}

void shiro::commands::manager::find_command_files(const fs::path &path) {
    fs::directory_iterator end;

    for (const auto &entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_directory())
            continue;

        if (!boost::algorithm::ends_with(entry.path().u8string(), ".py"))
            continue;

        python_command command(entry.path());
        command.populate();

        if (!command.is_sane()) {
            LOG_F(WARNING, "Command file %s failed sanity check. Skipping...", entry.path().c_str());
            continue;
        }

        commands.emplace_back(command);
    }
}

bool shiro::commands::manager::execute(const std::string &command, std::deque<std::string> &args, std::shared_ptr<users::user> user, std::string channel) {
    for (python_command cmd : commands) {
        if (cmd.command == command || std::find(cmd.aliases.begin(), cmd.aliases.end(), command) != cmd.aliases.end()) {
            cmd.execute(args, std::move(user), std::move(channel));
            return true;
        }
    }

    return false;
}
