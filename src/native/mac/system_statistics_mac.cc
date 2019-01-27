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

#include <stdexcept>

#include "../system_statistics.hh"

// TODO: requires implementation for Darwin, see https://stackoverflow.com/a/1911863/10245934

void shiro::native::system_stats::init() {
    throw std::runtime_error("init() not implemented on Darwin.");
}

uint64_t shiro::native::system_stats::get_total_memory() {
    throw std::runtime_error("get_total_memory() not implemented on Darwin.");
}

uint64_t shiro::native::system_stats::get_memory_usage() {
    throw std::runtime_error("get_memory_usage() not implemented on Darwin.");
}

uint64_t shiro::native::system_stats::get_process_memory_usage() {
    throw std::runtime_error("get_process_memory_usage() not implemented on Darwin.");
}

uint64_t shiro::native::system_stats::get_total_physical_memory() {
    throw std::runtime_error("get_total_physical_memory() not implemented on Darwin.");
}

uint64_t shiro::native::system_stats::get_physical_memory_usage() {
    throw std::runtime_error("get_physical_memory_usage() not implemented on Darwin.");
}

uint64_t shiro::native::system_stats::get_physical_process_memory_usage() {
    throw std::runtime_error("get_physical_process_memory_usage() not implemented on Darwin.");
}

double shiro::native::system_stats::get_cpu_usage() {
    throw std::runtime_error("get_cpu_usage() not implemented on Darwin.");
}

double shiro::native::system_stats::get_process_cpu_usage() {
    throw std::runtime_error("get_process_cpu_usage() not implemented on Darwin.");
}

#endif
