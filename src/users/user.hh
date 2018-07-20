#ifndef SHIRO_USER_HH
#define SHIRO_USER_HH

#include <chrono>

#include "../io/layouts/user/user_presence.hh"
#include "../io/layouts/user/user_stats.hh"
#include "../io/layouts/user/user_status.hh"
#include "../io/queue.hh"
#include "../shiro.hh"

namespace shiro::users {

    class user {
    public:
        int32_t user_id;
        std::string password;
        std::string salt;

        // Session info
        std::string token; // UUID v4
        std::string client_version;
        std::string utc_offset;
        std::string hwid; // SHA256
        std::chrono::seconds last_ping;

        io::layouts::user_presence presence;
        io::layouts::user_stats stats;
        io::layouts::user_status status;

        io::queue queue;

        explicit user(int32_t user_id);
        explicit user(const std::string &username);

        bool init();
        void update();

        bool check_password(const std::string &password);

    };

}

#endif //SHIRO_USER_HH
