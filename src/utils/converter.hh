#ifndef SHIRO_CONVERTER_HH
#define SHIRO_CONVERTER_HH

#include <cstdint>
#include <string>

namespace shiro::utils::converter {

    // std::string -> x

    bool str_to_byte(const std::string &in, uint8_t &out);

    bool str_to_short(const std::string &in, int16_t &out);

    bool str_to_int(const std::string &in, int32_t &out);

    bool str_to_ulong(const std::string &in, uint64_t &out);

    bool str_to_float(const std::string &in, float &out);

    // float -> x

    bool float_to_int(float in, int32_t &out);

    // double -> x

    bool double_to_int(double in, int32_t &out);

}

#endif //SHIRO_CONVERTER_HH
