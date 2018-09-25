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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/sysinfo.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/vtimes.h>

#include "../system_statistics.hh"

// This whole source file could get some cleanup and modernization.
// It was written at a time where the getrusage(2) POSIX syscall was
// not fully implemented yet. A interesting call may as well be
// sysinfo(2), which covers most of the methods in this file.

static uint64_t last_total_user;
static uint64_t last_total_user_low;
static uint64_t last_total_sys;
static uint64_t last_total_idle;

static clock_t last_cpu;
static clock_t last_sys_cpu;
static clock_t last_user_cpu;

static int32_t thread_count = 0; // CPU threads

#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-result"
#endif

void shiro::native::system_stats::init() {
    FILE *file = std::fopen("/proc/stat", "r");
    std::fscanf(file, "cpu %lu %lu %lu %lu", &last_total_user, &last_total_user_low, &last_total_sys, &last_total_idle);
    std::fclose(file);

    struct tms time_sample;
    char buffer[128];

    last_cpu = times(&time_sample);
    last_sys_cpu = time_sample.tms_stime;
    last_user_cpu = time_sample.tms_utime;

    file = std::fopen("/proc/cpuinfo", "r");

    while (std::fgets(buffer, sizeof(buffer), file) != nullptr) {
        if (std::strncmp(buffer, "processor", 9) == 0)
            thread_count++;
    }

    std::fclose(file);
}

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif

uint64_t shiro::native::system_stats::get_total_memory() {
    struct sysinfo info;
    sysinfo(&info);

    uint64_t result = info.totalram;
    result += info.totalswap;
    result *= info.mem_unit;

    return result;
}

uint64_t shiro::native::system_stats::get_memory_usage() {
    struct sysinfo info;
    sysinfo(&info);

    uint64_t result = info.totalram - info.freeram;
    result += info.totalswap - info.freeswap;
    result *= info.mem_unit;

    return result;
}

uint64_t shiro::native::system_stats::get_process_memory_usage() {
    FILE *file = std::fopen("/proc/self/status", "r");

    if (file == nullptr)
        return 0;

    uint64_t result = 0;
    char buffer[128];

    while (std::fgets(buffer, sizeof(buffer), file) != nullptr) {
        if (std::strncmp(buffer, "VmSize:", 7) == 0) {
            result = [&]() -> uint64_t {
                const char *line = buffer;

                while (*line < '0' || *line > '9') {
                    line++;
                }

                buffer[std::strlen(buffer) - 3] = '\0';
                return std::atoi(line);
            }();
            break;
        }
    }

    std::fclose(file);
    return result;
}

uint64_t shiro::native::system_stats::get_total_physical_memory() {
    struct sysinfo info;
    sysinfo(&info);

    uint64_t result = info.totalram;
    result *= info.mem_unit;

    return result;
}

uint64_t shiro::native::system_stats::get_physical_memory_usage() {
    struct sysinfo info;
    sysinfo(&info);

    uint64_t result = info.totalram - info.freeram;
    result *= info.mem_unit;

    return result;
}

uint64_t shiro::native::system_stats::get_physical_process_memory_usage() {
    FILE *file = std::fopen("/proc/self/status", "r");

    if (file == nullptr)
        return 0;

    uint64_t result = 0;
    char buffer[128];

    while (std::fgets(buffer, sizeof(buffer), file) != nullptr) {
        if (std::strncmp(buffer, "VmRSS:", 6) == 0) {
            result = [&]() -> uint64_t {
                const char *line = buffer;

                while (*line < '0' || *line > '9') {
                    line++;
                }

                buffer[std::strlen(buffer) - 3] = '\0';
                return std::atoi(line);
            }();
            break;
        }
    }

    std::fclose(file);
    return result;
}

#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-result"
#endif

double shiro::native::system_stats::get_cpu_usage() {
    double percent;

    uint64_t total_user = 0;
    uint64_t total_user_low = 0;
    uint64_t total_sys = 0;
    uint64_t total_idle = 0;
    uint64_t total = 0;

    FILE *file = std::fopen("/proc/stat", "r");
    std::fscanf(file, "cpu %lu %lu %lu %lu", &total_user, &total_user_low, &total_sys, &total_idle);
    std::fclose(file);

    total = (total_user - last_total_user) + (total_user_low - last_total_user_low) + (total_sys - last_total_sys);
    percent = total;

    total += total_idle - last_total_idle;

    percent /= total;
    percent *= 100;

    last_total_user = total_user;
    last_total_user_low = total_user_low;
    last_total_sys = total_sys;
    last_total_idle = total_idle;

    return percent;
}

#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif

double shiro::native::system_stats::get_process_cpu_usage() {
    struct tms time_sample;
    clock_t time;
    double percent;

    time = times(&time_sample);

    percent = (time_sample.tms_stime - last_sys_cpu) + (time_sample.tms_utime - last_user_cpu);
    percent /= time + last_cpu;

    percent /= thread_count;
    percent *= 100;

    last_sys_cpu = time_sample.tms_stime;
    last_user_cpu = time_sample.tms_utime;
    last_cpu = time;

    return percent;
}

#endif
