#ifndef SHIRO_MESSAGE_HH
#define SHIRO_MESSAGE_HH

#include "../serializable.hh"

namespace shiro::io::layouts {

    class message : public serializable {
    public:
        message() = default;
        message(std::string sender, int32_t sender_id, std::string content, std::string channel);

        std::string sender = "";
        int32_t sender_id = 0;

        std::string content = "";
        std::string channel = "";

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;

    };

}

#endif //SHIRO_MESSAGE_HH
