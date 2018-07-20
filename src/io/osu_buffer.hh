#ifndef SHIRO_OSU_BUFFER_HH
#define SHIRO_OSU_BUFFER_HH

#include <cstdint>
#include <string>
#include <vector>

namespace shiro::io {

    class buffer {
    private:
        std::vector<uint8_t> bytes;

        size_t written_size = 0;
        unsigned int position = 0;

        void allocate(int size);
        void seek(unsigned int position);
        void advance(int size);

    public:
        buffer() = default;
        explicit buffer(const std::string &data);

        uint8_t read_byte();
        void write_byte(uint8_t value);

        int16_t read_short();
        void write_short(int16_t value);

        uint16_t read_ushort();
        void write_ushort(uint16_t value);

        int32_t read_int();
        void write_int(int32_t value);

        uint32_t read_uint();
        void write_uint(uint32_t value);

        int64_t read_long();
        void write_long(int64_t value);

        uint64_t read_ulong();
        void write_ulong(uint64_t value);

        float read_float();
        void write_float(float value);

        double read_double();
        void write_double(double value);

        std::string read_string();
        void write_string(std::string value);

        std::vector<int32_t> read_array();
        void write_array(std::vector<int32_t> value);

        void write_buffer(buffer &buffer);

        std::string serialize();
        bool can_read(size_t amount);

        bool is_empty();
        void reset();

    };

}

#endif //SHIRO_OSU_BUFFER_HH
