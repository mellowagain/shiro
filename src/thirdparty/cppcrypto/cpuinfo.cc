/******************************************************************************
This code is released under Simplified BSD License (see license.txt).
******************************************************************************/
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include "cpuinfo.hh"
#ifndef _MSC_VER
#include "cpuid.h"
#else
#include <intrin.h>
#endif

namespace cppcrypto {

    const cpu_info::cpu_info_impl cpu_info::impl_;

    using namespace std;
    cpu_info::cpu_info_impl::cpu_info_impl() : ecx1_{0}, edx1_{0}, ebx7_{0}, ecx7_{0}, ecx81_{0}, edx81_{0} {
#ifdef _MSC_VER
        std::array<int, 4> cpui;
#else
        std::array<unsigned int, 4> cpui;
#endif

#ifdef _MSC_VER
        __cpuid(cpui.data(), 0);
        int ids = cpui[0];
#else
        int                         ids = __get_cpuid_max(0, 0);
#endif

        if (ids >= 1) {
#ifdef _MSC_VER
            __cpuidex(cpui.data(), 1, 0);
#else
            __cpuid_count(1, 0, cpui[0], cpui[1], cpui[2], cpui[3]);
#endif
            ecx1_ = cpui[2];
            edx1_ = cpui[3];
        }

        if (ids >= 7) {
#ifdef _MSC_VER
            __cpuidex(cpui.data(), 7, 0);
#else
            __cpuid_count(7, 0, cpui[0], cpui[1], cpui[2], cpui[3]);
#endif
            ebx7_ = cpui[1];
            ecx7_ = cpui[2];
        }

#ifdef _MSC_VER
        __cpuid(cpui.data(), 0x80000000);
        unsigned int extended_ids = cpui[0];
#else
        unsigned int extended_ids = __get_cpuid_max(0x80000000, 0);
#endif

        if (extended_ids >= 0x80000001) {
#ifdef _MSC_VER
            __cpuidex(cpui.data(), 0x80000001, 0);
#else
            __cpuid_count(0x80000001, 0, cpui[0], cpui[1], cpui[2], cpui[3]);
#endif
            ecx81_ = cpui[2];
            edx81_ = cpui[3];
        }
    };
}
