#ifndef SHIRO_MESSAGE_HH
#define SHIRO_MESSAGE_HH

#include "../layout.hh"

namespace shiro::io::layouts {

    class message : public layout {
    public:
        message() = default;
        message(std::string sender, int32_t sender_id, std::string content, std::string channel);

        std::string sender;
        int32_t sender_id;

        std::string content;
        std::string channel;

        std::string marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

    };

}

#endif //SHIRO_MESSAGE_HH
