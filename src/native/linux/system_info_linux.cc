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

#if defined(__linux__)

#include <sys/utsname.h>
#include <unistd.h>
#include <limits.h>

#include "../system_info.hh"

std::string shiro::native::system_info::get_architecture() {
    struct utsname uname_data;
    uname(&uname_data);

    return uname_data.machine;
}

std::string shiro::native::system_info::get_host_name() {
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);

    return hostname;
}

std::string shiro::native::system_info::get_hw_model() {
    return "";
}

std::string shiro::native::system_info::get_os_version() {
    struct utsname uname_data;
    uname(&uname_data);

    return uname_data.release;
}

std::string shiro::native::system_info::get_os_build() {
    struct utsname uname_data;
    uname(&uname_data);

    return uname_data.version;
}

#endif
