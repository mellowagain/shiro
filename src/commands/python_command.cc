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

#include <python3.8/Python.h> // Python needs to be included before *any* system headers
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <utility>

#include "../thirdparty/loguru.hh"
#include "../utils/bot_utils.hh"
#include "../utils/python_utils.hh"
#include "python_command.hh"
#include "command_manager.hh"

shiro::commands::python_command::python_command(fs::path path) : path(std::move(path)) {
    // Initialized in initializer list
}

void shiro::commands::python_command::populate() {
    std::ifstream stream(this->path.u8string());

    if (!stream.is_open())
        return;

    std::string line;

    while (std::getline(stream, line)) {
        if (!boost::algorithm::starts_with(line, "# @"))
            continue;

        std::vector<std::string> parts;
        boost::split(parts, line, boost::is_any_of(" "));

        std::for_each(parts.begin(), parts.end(), [](std::string &part) {
            boost::algorithm::to_lower(part);
        });

        if (parts.size() < 3)
            continue;

        // Remove the "#" at the beginning
        parts.erase(parts.begin());

        // Skip the "@" at the begin of the tag
        std::string tag = parts.at(0).substr(1);

        // and remove it from the vector now that we have it
        parts.erase(parts.begin());

        // Join the rest to a single string for further usage
        std::string content = boost::algorithm::join(parts, " ");

        if (tag == "command") {
            if (content != boost::algorithm::to_lower_copy(this->module_name())) {
                LOG_F(ERROR, "@command tag does not match filename.");
                return;
            }

            this->command = content;
        } else if (tag == "alias") {
            this->aliases = parts;
        } else if (tag == "permission") {
            try {
                this->permission = boost::lexical_cast<uint64_t>(content);
            } catch (const boost::bad_lexical_cast &ex) {
                LOG_F(ERROR, "Unable to convert %s into valid permission (uint64_t): %s", content.c_str(), ex.what());
                return;
            }
        } else if (tag == "syntax") {
            this->syntax = content;
        } else if (tag == "description") {
            this->description = content;
        }
    }

    if (!this->is_sane())
        return;

    try {
        this->module = boost::python::import(boost::python::str(this->module_name().c_str()));
    } catch (const boost::python::error_already_set &ex) {
        LOG_F(ERROR, "Unable to import module %s into Python interpreter:", this->module_name().c_str());
        PyErr_Print();
    }

    // TODO: Reload the module if the file on disk changes

    LOG_F(INFO, "Found command !%s from %s file (located at %p)", this->command.c_str(), this->path.filename().c_str(), this->module.ptr());
}

void shiro::commands::python_command::execute(std::deque<std::string> &args, std::shared_ptr<users::user> user, std::string channel) {
    namespace python = boost::python;

    bool success = false;
    python::list python_args = utils::python::to_python_list(args);

    try {
        success = python::call_method<bool>(
                this->module.ptr(), "execute",
                python::ptr(commands::manager::wrapper.get()),
                python_args,
                python::ptr(user.get()),
                python::str(channel.c_str())
        );
    } catch (const python::error_already_set &ex) {
        utils::bot::respond("An internal server occurred while executing the command.", std::move(user), std::move(channel), true);

        LOG_F(ERROR, "Error occurred while executing command !%s:", this->module_name().c_str());
        PyErr_Print();
        return;
    }

    if (success)
        return;

    utils::bot::respond("Syntax: " + this->get_syntax(), std::move(user), std::move(channel));
}

std::string shiro::commands::python_command::module_name() {
    return this->path.stem().u8string();
}

std::string shiro::commands::python_command::get_syntax() {
    return "!" + this->command + " " + this->syntax;
}

bool shiro::commands::python_command::is_sane() {
    return !this->command.empty() && !this->path.empty();
}
