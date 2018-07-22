#ifndef SHIRO_SERIALIZABLE_HH
#define SHIRO_SERIALIZABLE_HH

#include "../osu_buffer.hh"

namespace shiro::io {

    class serializable {
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

        serializable() = default;

        explicit serializable(uint8_t data);
        explicit serializable(int16_t data);
        explicit serializable(uint16_t data);
        explicit serializable(int32_t data);
        explicit serializable(uint32_t data);
        explicit serializable(int64_t data);
        explicit serializable(uint64_t data);
        explicit serializable(float data);
        explicit serializable(double data);
        explicit serializable(std::string data);
        explicit serializable(std::vector<int32_t> data);

        virtual shiro::io::buffer marshal();
        virtual void unmarshal(buffer &buf);

        virtual int32_t get_size();

    };

}

#endif //SHIRO_SERIALIZABLE_HH
