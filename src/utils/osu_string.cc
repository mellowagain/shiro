#include "../io/osu_buffer.hh"
#include "leb128.hh"
#include "osu_string.hh"

std::string shiro::utils::osu_string(std::string value) {
    io::buffer buffer;

    buffer.write<uint8_t>(11);
    utils::leb128::write_leb128(buffer, value.size());
    buffer.write_string(value);

    return buffer.serialize();
}
