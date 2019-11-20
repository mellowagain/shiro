#ifndef SHIRO_REDIS_HH
#define SHIRO_REDIS_HH

#include <cpp_redis/cpp_redis>
#include <cstdint>
#include <memory>
#include <string>

namespace shiro {

    class redis {
    private:
        std::shared_ptr<cpp_redis::client> client = nullptr;

        std::string address;
        uint32_t port;
        std::string password;

    public:
        redis(std::string address, uint32_t port, std::string password = "");

        void connect();
        void disconnect();
        void setup();

        bool is_connected();
        std::shared_ptr<cpp_redis::client> get();

    };

}

#endif //SHIRO_REDIS_HH
