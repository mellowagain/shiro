#ifndef SHIRO_DATABASE_HH
#define SHIRO_DATABASE_HH

#include <string>

#if __has_include(mysql++.h>)
    #include <mysql++.h>
#else
    #define MYSQLPP_MYSQL_HEADERS_BURIED
    #include <mysql++/mysql++.h>
#endif

namespace shiro {

    class database {
    private:
        std::string address;
        unsigned int port;
        std::string db;

        std::string username;
        std::string password;

        mysqlpp::Connection connection = nullptr;

        database(const std::string &address, unsigned int port, const std::string &db, const std::string &username, const std::string &password);

    public:
        void connect();
        void disconnect();

        void setup();

        void update(const std::string &query_str);
        mysqlpp::StoreQueryResult query(const std::string &query_str);

        bool is_connected();
        mysqlpp::Connection get_connection();

    };

}

#endif //SHIRO_DATABASE_HH
