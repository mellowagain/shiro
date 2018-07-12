#ifndef SHIRO_LAYOUT_HH
#define SHIRO_LAYOUT_HH

#include "../osu_buffer.hh"

namespace shiro::io {

    class layout {
    protected:
        enum class data_type : int {
            byte = 0,
            int16 = 1,
            uint16 = 2,
            int32 = 3,
            uint32 = 4,
            int64 = 5,
            uint64 = 6,
            flt = 7,
            dble = 8,
            string = 9,
            array = 10
        };

    private:
        data_type type = data_type::byte;

    public:
        uint8_t data_byte = 0;
        int16_t data_short = 0;
        uint16_t data_ushort = 0;
        int32_t data_int = 0;
        uint32_t data_uint = 0;
        int64_t data_long = 0;
        uint64_t data_ulong = 0;
        float data_float = 0.0f;
        double data_double = 0.0;
        std::string data_string;
        std::vector<int32_t> data_array;

        layout() = default;

        explicit layout(uint8_t data);
        explicit layout(int16_t data);
        explicit layout(uint16_t data);
        explicit layout(int32_t data);
        explicit layout(uint32_t data);
        explicit layout(int64_t data);
        explicit layout(uint64_t data);
        explicit layout(float data);
        explicit layout(double data);
        explicit layout(std::string data);
        explicit layout(std::vector<int32_t> data);

        virtual size_t get_size();
        virtual std::string marshal();
        virtual void unmarshal(shiro::io::buffer &buffer);

    };

}

#endif //SHIRO_LAYOUT_HH
