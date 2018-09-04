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
