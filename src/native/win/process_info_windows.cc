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

#if defined(_WIN32)

// windows.h needs to be included before processthreadsapi.h because of WinAPI weirdness
#include <windows.h>
#include <processthreadsapi.h>

#include "../process_info.hh"

int32_t shiro::native::process_info::get_pid() {
    return GetCurrentProcessId();
}

std::string shiro::native::process_info::get_executable_location() {
    char buffer[MAX_PATH];
    HMODULE handle = GetModuleHandle(nullptr);

    if (handle == nullptr)
        return "shiro.exe"; // Fallback to our generic executable name

    GetModuleFileName(handle, buffer, sizeof(buffer));

    return buffer;
}

void shiro::native::process_info::set_env(const std::string &key, const std::string &value) {
    _putenv_s(key.c_str(), value.c_str());
}

#endif
