#include <utility>

#include "../../utils/leb128.hh"
#include "../../utils/osu_string.hh"
#include "layout.hh"

shiro::io::layout::layout(uint8_t data) {
    this->type = data_type::byte;
    this->data_byte = data;
}

shiro::io::layout::layout(int16_t data) {
    this->type = data_type::int16;
    this->data_short = data;
}

shiro::io::layout::layout(uint16_t data) {
    this->type = data_type::uint16;
    this->data_ushort = data;
}

shiro::io::layout::layout(int32_t data) {
    this->type = data_type::int32;
    this->data_int = data;
}

shiro::io::layout::layout(uint32_t data) {
    this->type = data_type::uint32;
    this->data_uint = data;
}

shiro::io::layout::layout(int64_t data) {
    this->type = data_type::int64;
    this->data_long = data;
}

shiro::io::layout::layout(uint64_t data) {
    this->type = data_type::uint64;
    this->data_ulong = data;
}

shiro::io::layout::layout(float data) {
    this->type = data_type::flt;
    this->data_float = data;
}

shiro::io::layout::layout(double data) {
    this->type = data_type::dble;
    this->data_double = data;
}

shiro::io::layout::layout(std::string data) {
    this->type = data_type::string;
    this->data_string = std::move(data);
}

shiro::io::layout::layout(std::vector<int32_t> data) {
    this->type = data_type::array;
    this->data_array = std::move(data);
}

size_t shiro::io::layout::get_size() {
    switch (this->type) {
        case data_type::byte:
            return 1;
        case data_type::int16:
        case data_type::uint16:
            return 2;
        case data_type::int32:
        case data_type::uint32:
            return 4;
        case data_type::int64:
        case data_type::uint64:
            return 8;
        case data_type::flt:
            return 4;
        case data_type::dble:
            return 8;
        case data_type::string:
            return this->data_string.length();
        case data_type::array:
            return this->data_array.size();
    }
}

std::string shiro::io::layout::marshal() {
    buffer buf;

    switch (this->type) {
        case data_type::byte:
            buf.write_int(this->get_size());
            buf.write_byte(this->data_byte);
            break;
        case data_type::int16:
            buf.write_int(this->get_size());
            buf.write_short(this->data_short);
            break;
        case data_type::uint16:
            buf.write_int(this->get_size());
            buf.write_ushort(this->data_ushort);
            break;
        case data_type::int32:
            buf.write_int(this->get_size());
            buf.write_int(this->data_int);
            break;
        case data_type::uint32:
            buf.write_int(this->get_size());
            buf.write_uint(this->data_uint);
            break;
        case data_type::int64:
            buf.write_int(this->get_size());
            buf.write_long(this->data_long);
            break;
        case data_type::uint64:
            buf.write_int(this->get_size());
            buf.write_ulong(this->data_ulong);
            break;
        case data_type::flt:
            buf.write_int(this->get_size());
            buf.write_float(this->data_float);
            break;
        case data_type::dble:
            buf.write_int(this->get_size());
            buf.write_double(this->data_double);
            break;
        case data_type::string: {
            std::string osu_string = utils::osu_string(this->data_string);

            buf.write_int(osu_string.length());
            buf.write_string(osu_string);

            break;
        }
        case data_type::array:
            buf.write_int(this->data_array.size());
            buf.write_array(this->data_array);
            break;
    }

    return buf.serialize();
}

void shiro::io::layout::unmarshal(shiro::io::buffer &buffer) {
    data_type type = (data_type) buffer.read_int();

    switch (type) {
        case data_type::byte:
            this->data_byte = buffer.read_byte();
            break;
        case data_type::int16:
            this->data_short = buffer.read_short();
            break;
        case data_type::uint16:
            this->data_ushort = buffer.read_ushort();
            break;
        case data_type::int32:
            this->data_int = buffer.read_int();
            break;
        case data_type::uint32:
            this->data_uint = buffer.read_uint();
            break;
        case data_type::int64:
            this->data_long = buffer.read_long();
            break;
        case data_type::uint64:
            this->data_ulong = buffer.read_ulong();
            break;
        case data_type::flt:
            this->data_float = buffer.read_float();
            break;
        case data_type::dble:
            this->data_double = buffer.read_double();
            break;
        case data_type::string:
            this->data_string = buffer.read_string();
            break;
        case data_type::array:
            this->data_array = buffer.read_array();
            break;
    }
}
