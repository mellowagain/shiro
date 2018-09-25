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

#include <cstring>
#include <Windows.h>
#include <pdh.h>
#include <Psapi.h>
#include <TCHAR.h>

#include "../system_statistics.hh"

static PDH_HQUERY cpu_query;
static PDH_HCOUNTER cpu_total;

static ULARGE_INTEGER last_cpu;
static ULARGE_INTEGER last_sys_cpu;
static ULARGE_INTEGER last_user_cpu;

static int32_t thread_count = 0; // CPU threads

static HANDLE self_handle;

void shiro::native::system_stats::init() {
    PdhOpenQuery(nullptr, nullptr, &cpu_query);
    PdhAddEnglishCounter(cpu_query, L"\\Processor(_Total)\\% Processor Time", nullptr, &cpu_total);
    PdhCollectQueryData(cpu_query);

    SYSTEM_INFO sys_info;
    FILETIME file_time;
    FILETIME sys_time;
    FILETIME user_time;

    GetSystemInfo(&sys_info);
    thread_count = sys_info.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&file_time);
    std::memcpy(&last_cpu, &file_time, sizeof(FILETIME));

    self_handle = GetCurrentProcess();

    GetProcessTimes(self_handle, &file_time, &file_time, &sys_time, &user_time);
    std::memcpy(&last_sys_cpu, &sys_time, sizeof(FILETIME));
    std::memcpy(&last_user_cpu, &user_time, sizeof(FILETIME));
}

uint64_t shiro::native::system_stats::get_total_memory() {
    MEMORYSTATUSEX memory_status;
    memory_status.dwLength = sizeof(MEMORYSTATUSEX);

    GlobalMemoryStatusEx(&memory_status);

    return memory_status.ullTotalPageFile;
}

uint64_t shiro::native::system_stats::get_memory_usage() {
    MEMORYSTATUSEX memory_status;
    memory_status.dwLength = sizeof(MEMORYSTATUSEX);

    GlobalMemoryStatusEx(&memory_status);

    return memory_status.ullTotalPageFile - memory_status.ullAvailPageFile;
}

uint64_t shiro::native::system_stats::get_process_memory_usage() {
    PROCESS_MEMORY_COUNTERS_EX process_memory_counters;

    GetProcessMemoryInfo(GetCurrentProcess(), &process_memory_counters, sizeof(PROCESS_MEMORY_COUNTERS_EX));

    return process_memory_counters.PrivateUsage;
}

uint64_t shiro::native::system_stats::get_total_physical_memory() {
    MEMORYSTATUSEX memory_status;
    memory_status.dwLength = sizeof(MEMORYSTATUSEX);

    GlobalMemoryStatusEx(&memory_status);

    return memory_status.ullTotalPhys;
}

uint64_t shiro::native::system_stats::get_physical_memory_usage() {
    MEMORYSTATUSEX memory_status;
    memory_status.dwLength = sizeof(MEMORYSTATUSEX);

    GlobalMemoryStatusEx(&memory_status);

    return memory_status.ullTotalPhys - memory_status.ullAvailPhys;
}

uint64_t shiro::native::system_stats::get_physical_process_memory_usage() {
    PROCESS_MEMORY_COUNTERS_EX process_memory_counters;

    GetProcessMemoryInfo(GetCurrentProcess(), &process_memory_counters, sizeof(PROCESS_MEMORY_COUNTERS_EX));

    return process_memory_counters.WorkingSetSize;
}

double shiro::native::system_stats::get_cpu_usage() {
    PDH_FMT_COUNTERVALUE counter_value;

    PdhCollectQueryData(cpu_query);
    PdhGetFormattedCounterValue(cpu_total, PDH_FMT_DOUBLE, nullptr, &counter_value);

    return counter_value.doubleValue;
}

double shiro::native::system_stats::get_process_cpu_usage() {
    FILETIME file_time;
    FILETIME sys_time;
    FILETIME user_time;
    ULARGE_INTEGER now;
    ULARGE_INTEGER sys;
    ULARGE_INTEGER user;
    double percent;

    GetSystemTimeAsFileTime(&file_time);
    std::memcpy(&now, &file_time, sizeof(FILETIME));

    GetProcessTimes(self, &file_time, &file_time, &sys_time, &user_time);
    std::memcpy(&sys, &sys_time, sizeof(FILETIME));
    std::memcpy(&user, &user_time, sizeof(FILETIME));

    percent = (sys.QuadPart - last_cpu.QuadPart) + (user.QuadPart - last_cpu.QuadPart);
    percent /= now.QuadPart - last_cpu.QuadPart;
    percent /= thread_count;
    percent *= 100;

    last_cpu = now;
    last_sys_cpu = sys;
    last_user_cpu = user;

    return percent;
}

#endif
