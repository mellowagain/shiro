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

#if defined(_WIN32)

#include <windows.h>
#include <intrin.h>
#include <cstring>

#include "../system_info.hh"

std::string shiro::native::system_info::get_architecture() {
    bool is_64bit = []() {
        int cpu_info[4];
        __cpuid(cpu_info, 0);
        return (cpu_info[3] & 0x20000000) || false;
    }();

    return is_64bit ? "x86_64" : "x86";
}

std::string shiro::native::system_info::get_host_name() {
    char buffer[32767];
    unsigned int char_count = 32767;

    if (!GetComputerName(buffer, &char_count))
        return "";

    return buffer;
}

std::string shiro::native::system_info::get_hw_model() {
    return "";
}

std::string shiro::native::system_info::get_os_version() {
    OSVERSIONINFOEX info;
    std::memset(&info, 0, sizeof(OSVERSIONINFOEX));
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    GetVersionEx(&info);

    return std::to_string(info.dwMajorVersion) + "." + std::to_string(info.dwMinorVersion) + "." + std::to_string(info.dwBuildNumber);
}

std::string shiro::native::system_info::get_os_build() {
    OSVERSIONINFOEX info;
    std::memset(&info, 0, sizeof(OSVERSIONINFOEX));
    info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    GetVersionEx(&info);

    return std::to_string(info.dwBuildNumber);
}

#endif
