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

#if defined(__APPLE__)

#include <sys/syslimits.h>
#include <unistd.h>

#include "../../utils/filesystem.hh"
#include "../process_info.hh"

int32_t shiro::native::process_info::get_pid() {
    return getpid();
}

std::string shiro::native::process_info::get_executable_location() {
    char buffer[PATH_MAX + 1];

    #ifdef ARCH_darwin_14_i86
        uint32_t size = sizeof(buffer);

        if (_NSGetExecutablePath(buffer, &size) != 0)
            return "shiro"; // Fallback to our generic executable name
    #else
        return fs::read_symlink("/proc/self/exe").u8string();
    #endif

    return buffer;
}

void shiro::native::process_info::set_env(const std::string &key, const std::string &value) {
    setenv(key.c_str(), value.c_str(), 1);
}

#endif
