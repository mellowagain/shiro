#ifndef SHIRO_LEB128_HH
#define SHIRO_LEB128_HH

#include <cstdint>
#include "../io/osu_buffer.hh"

namespace shiro::utils::leb128 {
    
    uint64_t read_leb128(shiro::io::buffer &buffer);
    void write_leb128(shiro::io::buffer &buffer, uint64_t value);

}

#endif //SHIRO_LEB128_HH
