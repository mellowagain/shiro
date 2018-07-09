#include "leb128.hh"

uint64_t shiro::utils::leb128::read_leb128(shiro::io::buffer &buffer) {
    uint64_t total = 0;
    int shift = 0;

    uint8_t byte = buffer.read_byte();

    if ((byte & 0x80) == 0) {
        total |= ((byte & 0x7F) << shift);
    } else {
        bool end = false;

        do {
            if (shift) {
                byte = buffer.read_byte();
            }

            total |= ((byte & 0x7F) << shift);

            if ((byte & 0x80) == 0)
                end = true;

            shift += 7;
        } while (!end);
    }

    return total;
}

void shiro::utils::leb128::write_leb128(shiro::io::buffer &buffer, uint64_t value) {
    if (value == 0) {
        buffer.write_byte(static_cast<std::uint8_t>('\0'));
    } else {
        std::string output;

        do {
            uint8_t byte;

            byte = value & 0x7F;
            value >>= 7;

            if (value != 0) {
                byte |= 0x80;
            }

            buffer.write_byte(byte);
        } while (value > 0);
    }
}
