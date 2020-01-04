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
#include "../users/user.hh"
#include "../users/user_manager.hh"
#include "bot_utils.hh"
#include "python_utils.hh"

BOOST_PYTHON_MODULE(shiro)
{
    using namespace shiro;
    namespace python = boost::python;

    python::class_<io::layouts::user_presence>("user_presence", python::no_init)
            .def_readonly("user_id", &io::layouts::user_presence::user_id)
            .def_readonly("username", &io::layouts::user_presence::username)

            .def_readonly("time_zone", &io::layouts::user_presence::time_zone)
            .def_readonly("country_id", &io::layouts::user_presence::country_id)

            .def_readonly("permissions", &io::layouts::user_presence::permissions)

            .def_readonly("longitude", &io::layouts::user_presence::user_id)
            .def_readonly("latitude", &io::layouts::user_presence::user_id)

            .def_readonly("rank", &io::layouts::user_presence::rank)
    ;

    python::class_<io::layouts::user_stats>("user_stats", python::no_init)
            .def_readonly("user_id", &io::layouts::user_stats::user_id)

            .def_readonly("activity", &io::layouts::user_stats::activity)
            .def_readonly("activity_desc", &io::layouts::user_stats::activity_desc)

            .def_readonly("beatmap_checksum", &io::layouts::user_stats::beatmap_checksum)

            .def_readonly("current_mods", &io::layouts::user_stats::current_mods)
            .def_readonly("play_mode", &io::layouts::user_stats::play_mode)

            .def_readonly("beatmap_id", &io::layouts::user_stats::beatmap_id)

            .def_readonly("ranked_score", &io::layouts::user_stats::ranked_score)

            .def_readonly("max_combo", &io::layouts::user_stats::max_combo)
            .def_readonly("accuracy", &io::layouts::user_stats::accuracy)
            .def_readonly("play_count", &io::layouts::user_stats::play_count)
            .def_readonly("total_score", &io::layouts::user_stats::total_score)

            .def_readonly("rank", &io::layouts::user_stats::rank)
            .def_readonly("pp", &io::layouts::user_stats::pp)
    ;

    python::class_<io::layouts::user_status>("user_status", python::no_init)
            .def_readonly("activity", &io::layouts::user_status::activity)
            .def_readonly("activity_desc", &io::layouts::user_status::activity_desc)

            .def_readonly("beatmap_checksum", &io::layouts::user_status::beatmap_checksum)

            .def_readonly("current_mods", &io::layouts::user_status::current_mods)
            .def_readonly("play_mode", &io::layouts::user_status::play_mode)

            .def_readonly("beatmap_id", &io::layouts::user_status::beatmap_id)
    ;

    python::class_<users::user>("user", python::init<int32_t>())
            .def(python::init<std::string>())

            // Member variables
            .def_readonly("user_id", &users::user::user_id)
            .def_readonly("country", &users::user::country)
            .def_readonly("roles", &users::user::roles)
            //.def_readonly("friends", &users::user::friends)
            //.def_readonly("last_ping", &users::user::last_ping)
            .def_readwrite("hidden", &users::user::hidden)
            .def_readonly("client_version", &users::user::client_version)
            .def_readonly("client_build", &users::user::client_build)
            .def_readonly("client_type", &users::user::client_type)

            .def_readonly("presence", &users::user::presence)
            .def_readonly("stats", &users::user::stats)
            .def_readonly("status", &users::user::status)

            // Functions
            .def("init", &users::user::init)
            .def("update", &users::user::update)
            .def("save_stats", &users::user::save_stats)
            .def("refresh_stats", &users::user::refresh_stats)
            .def("get_url", &users::user::get_url)
    ;

    python::class_<utils::python::wrapper>("shiro_wrapper", python::no_init)
            .def("write", &utils::python::wrapper::write)
            .def("write_global", &utils::python::wrapper::write_global);

    python::def("write_bot", utils::bot::respond,
            python::arg("message"),
            python::arg("user"),
            python::arg("channel"));
}

void shiro::utils::python::wrapper::write(std::string message, int32_t user_id, std::string channel) {
    utils::bot::respond(std::move(message), users::manager::get_user_by_id(user_id), std::move(channel), true);
}

void shiro::utils::python::wrapper::write_global(std::string message, std::string channel) {
    utils::bot::respond(std::move(message), nullptr, std::move(channel), false);
}

void shiro::utils::python::init() {
    try {
        PyInit_shiro();
    } catch (const boost::python::error_already_set &ex) {
        LOG_F(ERROR, "Error initializing Shiro module for Python:");
        PyErr_Print();
    }
}
