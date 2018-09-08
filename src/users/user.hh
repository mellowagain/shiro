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
        int32_t user_id = 0;
        std::string password = "";
        std::string salt = "";
        std::string country = "XX";

        std::vector<int32_t> friends {};

        // Session info
        std::string token = ""; // UUID v4
        std::string client_version = ""; // Full client version string sent on login (b20160403.6)
        int32_t client_build = 20131216; // Fixed client build version (20160403)
        std::string hwid = ""; // SHA256
        std::chrono::seconds last_ping;

        io::layouts::user_presence presence;
        io::layouts::user_stats stats;
        io::layouts::user_status status;

        io::queue queue;

        explicit user(int32_t user_id);
        explicit user(const std::string &username);

        bool init();
        void update();
        void save_stats();

        // Re-sends presence and stats to the user to refresh on client-side
        void refresh_stats();

        bool check_password(const std::string &password);

    };

}

#endif //SHIRO_USER_HH
