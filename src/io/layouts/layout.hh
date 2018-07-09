#ifndef SHIRO_LAYOUT_HH
#define SHIRO_LAYOUT_HH

#include "../osu_buffer.hh"

namespace shiro::io {

    class layout {
    protected:
        enum class data_type {
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
        data_type type;

    public:
        uint8_t data_byte;
        int16_t data_short;
        uint16_t data_ushort;
        int32_t data_int;
        uint32_t data_uint;
        int64_t data_long;
        uint64_t data_ulong;
        float data_float;
        double data_double;
        std::string data_string;
        std::vector<int32_t> data_array;

        layout(uint8_t data);
        layout(int16_t data);
        layout(uint16_t data);
        layout(int32_t data);
        layout(uint32_t data);
        layout(int64_t data);
        layout(uint64_t data);
        layout(float data);
        layout(double data);
        layout(std::string data);
        layout(std::vector<int32_t> data);

        virtual size_t get_size();
        virtual std::string marshal();
        virtual void unmarshal(shiro::io::buffer &buffer);

    };

}

#endif //SHIRO_LAYOUT_HH
