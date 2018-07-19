#include <sstream>

#include "../utils/leb128.hh"
#include "osu_buffer.hh"

shiro::io::buffer::buffer(const std::string &data) {
    for (char c : data) {
        this->write_byte(c);
    }
}

void shiro::io::buffer::allocate(int size) {
    for (int i = 0; i < size; i++) {
        this->bytes.push_back(0x0);
    }
}

void shiro::io::buffer::seek(unsigned int position) {
    this->position = position;
}

void shiro::io::buffer::advance(int size) {
    this->position += size;
}

uint8_t shiro::io::buffer::read_byte() {
    return this->bytes.at(this->position++);
}

void shiro::io::buffer::write_byte(uint8_t value) {
    this->allocate(1);
    this->bytes.at(this->written_size++) = value;
}

int16_t shiro::io::buffer::read_short() {
    int16_t data = *reinterpret_cast<int16_t*>((uintptr_t) this->bytes.data() + this->position);
    this->advance(2);

    return data;
}

void shiro::io::buffer::write_short(int16_t value) {
    this->allocate(2);
    uint8_t *data = reinterpret_cast<uint8_t*>(&value);

    for (size_t i = 0; i < sizeof(int16_t); i++) {
        this->bytes.at(this->written_size++) = data[i];
    }
}

uint16_t shiro::io::buffer::read_ushort() {
    uint16_t data = *reinterpret_cast<uint16_t*>((uintptr_t) this->bytes.data() + this->position);
    this->advance(2);

    return data;
}

void shiro::io::buffer::write_ushort(uint16_t value) {
    this->allocate(2);
    uint8_t *data = reinterpret_cast<uint8_t*>(&value);

    for (size_t i = 0; i < sizeof(uint16_t); i++) {
        this->bytes.at(this->written_size++) = data[i];
    }
}

int32_t shiro::io::buffer::read_int() {
    int32_t data = *reinterpret_cast<int32_t*>((uintptr_t) this->bytes.data() + this->position);
    this->advance(4);

    return data;
}

void shiro::io::buffer::write_int(int32_t value) {
    this->allocate(4);
    uint8_t *data = reinterpret_cast<uint8_t*>(&value);

    for (size_t i = 0; i < sizeof(int32_t); i++) {
        this->bytes.at(this->written_size++) = data[i];
    }
}

uint32_t shiro::io::buffer::read_uint() {
    uint32_t data = *reinterpret_cast<uint32_t*>((uintptr_t) this->bytes.data() + this->position);
    this->advance(4);

    return data;
}

void shiro::io::buffer::write_uint(uint32_t value) {
    this->allocate(4);
    uint8_t *data = reinterpret_cast<uint8_t*>(&value);

    for (size_t i = 0; i < sizeof(uint32_t); i++) {
        this->bytes.at(this->written_size++) = data[i];
    }
}

int64_t shiro::io::buffer::read_long() {
    int64_t data = *reinterpret_cast<uint32_t*>((int64_t) this->bytes.data() + this->position);
    this->advance(8);

    return data;
}

void shiro::io::buffer::write_long(int64_t value) {
    this->allocate(8);
    uint8_t *data = reinterpret_cast<uint8_t*>(&value);

    for (size_t i = 0; i < sizeof(int64_t); i++) {
        this->bytes.at(this->written_size++) = data[i];
    }
}

uint64_t shiro::io::buffer::read_ulong() {
    uint64_t data = *reinterpret_cast<uint64_t*>((uintptr_t) this->bytes.data() + this->position);
    this->advance(8);

    return data;
}

void shiro::io::buffer::write_ulong(uint64_t value) {
    this->allocate(8);
    uint8_t *data = reinterpret_cast<uint8_t*>(&value);

    for (size_t i = 0; i < sizeof(uint64_t); i++) {
        this->bytes.at(this->written_size++) = data[i];
    }
}

float shiro::io::buffer::read_float() {
    float data = *reinterpret_cast<float*>((uintptr_t) this->bytes.data() + this->position);
    this->advance(4);

    return data;
}

void shiro::io::buffer::write_float(float value) {
    this->allocate(4);
    uint8_t *data = reinterpret_cast<uint8_t*>(&value);

    for (size_t i = 0; i < sizeof(float); i++) {
        this->bytes.at(this->written_size++) = data[i];
    }
}

double shiro::io::buffer::read_double() {
    double data = *reinterpret_cast<double*>((uintptr_t) this->bytes.data() + this->position);
    this->advance(8);

    return data;
}

void shiro::io::buffer::write_double(double value) {
    this->allocate(8);
    uint8_t *data = reinterpret_cast<uint8_t*>(&value);

    for (size_t i = 0; i < sizeof(double); i++) {
        this->bytes.at(this->written_size++) = data[i];
    }
}

std::string shiro::io::buffer::read_string() {
    if (this->read_byte() != 11)
        return "";

    std::string result;
    uint64_t length = shiro::utils::leb128::read_leb128(*this);

    for (int i = 0; i < static_cast<int>(length); i++) {
        result += static_cast<char>(this->read_byte());
    }

    return result;
}

void shiro::io::buffer::write_string(std::string value) {
    this->allocate(value.length());

    for (char &c : value) {
        this->bytes.at(this->written_size++) = static_cast<uint8_t>(c);
    }
}

std::vector<int32_t> shiro::io::buffer::read_array() {
    int size = this->read_int();
    std::vector<int32_t> result;

    for (int i = 0; i < size; i++) {
        result.push_back(this->read_int());
    }

    return result;
}

void shiro::io::buffer::write_array(std::vector<int32_t> value) {
    for (int32_t &data : value) {
        this->write_int(data);
    }
}

std::string shiro::io::buffer::serialize() {
    std::stringstream stream;

    for (std::size_t i{}; i < this->written_size; i++) {
        stream << std::hex << this->bytes.at(i);
    }

    return stream.str();
}

bool shiro::io::buffer::can_read(size_t amount) {
    return this->position + amount <= this->written_size;
}
