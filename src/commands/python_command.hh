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

#ifndef SHIRO_PYTHON_COMMAND_HH
#define SHIRO_PYTHON_COMMAND_HH

#include <boost/python.hpp>
#include <string>
#include <vector>

#include "../utils/filesystem.hh"
#include "../users/user.hh"

namespace shiro::commands {

    class python_command {
    private:
        fs::path path;
        boost::python::object module;

    public:
        std::string command;
        std::vector<std::string> aliases;
        uint64_t permission = 0;
        std::string syntax;
        std::string description;

    public:
        explicit python_command(fs::path path);

        void populate();
        void execute(std::deque<std::string> &args, std::shared_ptr<users::user> user, std::string channel);

        std::string module_name();
        std::string get_syntax();

        bool is_sane();

    };

}

#endif //SHIRO_PYTHON_COMMAND_HH
