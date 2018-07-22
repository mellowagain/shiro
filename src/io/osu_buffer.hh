#ifndef SHIRO_OSU_BUFFER_HH
#define SHIRO_OSU_BUFFER_HH

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace shiro::io {

    class buffer {
    private:
        std::vector<uint8_t> bytes;

        size_t written_size = 0;
        size_t position = 0;

        void allocate(size_t amount);

    public:
        buffer() = default;
        buffer(const buffer &buf);

        buffer(buffer &buf);
        explicit buffer(std::string data);

        void append(std::string data);
        void append(buffer &buf);

        template <typename t = uint8_t>
        void write(t data) {
            this->allocate(sizeof(t));
            uint8_t *data_arr = reinterpret_cast<uint8_t*>(&data);

            for (size_t i = 0; i < sizeof(t); i++)
                this->bytes.at(this->written_size++) = data_arr[i];
        }

        template <typename t = uint8_t>
        t read() {
            t data = *reinterpret_cast<t*>((uintptr_t)this->bytes.data() + this->position);
            this->position += sizeof(t);

            return data;
        }

        void write_string(std::string data);
        void write_array(std::vector<int32_t> data);

        std::string read_string();
        std::vector<int32_t> read_array();

        std::string serialize();

        bool can_read(size_t amount);
        bool is_empty();
        void clear();

        size_t get_size();

    };

}

#endif //SHIRO_OSU_BUFFER_HH
