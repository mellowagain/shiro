#ifndef SHIRO_CHANNEL_HH
#define SHIRO_CHANNEL_HH

#include "../layout.hh"

namespace shiro::io::layouts {

    class channel : public layout {
    public:
        channel(uint32_t id, bool auto_join, std::string name, std::string description, int32_t user_count);

        // Not written
        uint32_t id = 0;
        bool auto_join = false;

        // Written
        std::string name;
        std::string description;
        int32_t user_count;

        std::string marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        bool operator==(const channel &other_channel) const;

    };

}

namespace std {

    template <>
    struct hash<shiro::io::layouts::channel> {

        size_t operator()(const shiro::io::layouts::channel &channel) const;

    };

}

#endif //SHIRO_CHANNEL_HH
