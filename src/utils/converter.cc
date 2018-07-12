#include <cerrno>
#include <cfenv>
#include <cmath>

#include "converter.hh"

bool shiro::utils::converter::str_to_byte(const std::string &in, uint8_t &out) {
    unsigned long converted = std::strtoul(in.c_str(), nullptr, 2);

    if (errno == ERANGE/* || converted == 0*/)
        return false;

    if (converted > std::numeric_limits<uint8_t>::max())
        return false;

    out = (uint8_t) converted;
    return true;
}

bool shiro::utils::converter::str_to_short(const std::string &in, int16_t &out) {
    long converted = std::strtol(in.c_str(), nullptr, 2);

    if (errno == ERANGE/* || converted == 0*/)
        return false;

    if (converted > std::numeric_limits<int16_t>::max() || converted < std::numeric_limits<int16_t>::min())
        return false;

    out = (int16_t) converted;
    return true;
}

bool shiro::utils::converter::str_to_int(const std::string &in, int32_t &out) {
    long converted = std::strtol(in.c_str(), nullptr, 2);

    if (errno == ERANGE/* || converted == 0*/)
        return false;

    if (converted > std::numeric_limits<int32_t>::max() || converted < std::numeric_limits<int32_t>::min())
        return false;

    out = (int32_t) converted;
    return true;
}

bool shiro::utils::converter::str_to_ulong(const std::string &in, uint64_t &out) {
    long converted = std::strtoul(in.c_str(), nullptr, 2);

    if (errno == ERANGE/* || converted == 0*/)
        return false;

    if (converted > std::numeric_limits<int64_t>::max())
        return false;

    out = (uint64_t) converted;
    return true;
}

bool shiro::utils::converter::str_to_float(const std::string &in, float &out) {
    float converted = std::strtof(in.c_str(), nullptr);

    if (converted == HUGE_VALF/* || converted == 0.0f*/)
        return false;

    out = converted;
    return true;
}

bool shiro::utils::converter::float_to_int(float in, int32_t &out) {
    std::feclearexcept(FE_ALL_EXCEPT);

    float converted = std::round(in);

    if (std::fetestexcept(FE_INEXACT) || std::fetestexcept(FE_INVALID))
        return false;

    if (converted > std::numeric_limits<int32_t>::max() || converted < std::numeric_limits<int32_t>::min())
        return false;

    out = (int32_t) converted;
    return true;
}

bool shiro::utils::converter::double_to_int(double in, int32_t &out) {
    std::feclearexcept(FE_ALL_EXCEPT);

    double converted = std::round(in);

    if (std::fetestexcept(FE_INEXACT) || std::fetestexcept(FE_INVALID))
        return false;

    if (converted > std::numeric_limits<int32_t>::max() || converted < std::numeric_limits<int32_t>::min())
        return false;

    out = (int32_t) converted;
    return true;
}
