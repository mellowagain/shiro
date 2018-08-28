#ifndef SHIRO_SYSTEM_STATISTICS_HH
#define SHIRO_SYSTEM_STATISTICS_HH

#include <cstdint>

namespace shiro::native::system_stats {

    void init();

    // Virtual memory

    uint64_t get_total_memory();

    uint64_t get_memory_usage();

    uint64_t get_process_memory_usage();

    // Physical memory

    uint64_t get_total_physical_memory();

    uint64_t get_physical_memory_usage();

    uint64_t get_physical_process_memory_usage();

    // CPU

    double get_cpu_usage();

    double get_process_cpu_usage();

}

#endif  // SHIRO_SYSTEM_STATISTICS_HH
