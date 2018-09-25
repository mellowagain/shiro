#ifndef SHIRO_DATABASE_HH
#define SHIRO_DATABASE_HH

#include <sqlpp11/mysql/mysql.h>
#include <sqlpp11/sqlpp11.h>

#include <memory>
#include <string>

namespace shiro {

    class database {
    private:
        std::shared_ptr<sqlpp::mysql::connection_config> config = nullptr;

        std::string address;
        uint32_t port;
        std::string db;

        std::string username;
        std::string password;

    public:
        database(const std::string &address, uint32_t port, const std::string &db, const std::string &username, const std::string &password);

        void connect();
        void setup();

        bool is_connected(bool abort = false);
        std::shared_ptr<sqlpp::mysql::connection_config> get_config();

    };

}

#define is_query_empty(x) [&]() -> bool {                   \
    for ([[maybe_unused]] const auto &_ : x) {              \
        return false;                                       \
    }                                                       \
                                                            \
    return true;                                            \
}();                                                        \

#endif //SHIRO_DATABASE_HH
