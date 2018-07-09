#ifndef SHIRO_DATABASE_HH
#define SHIRO_DATABASE_HH

#include <string>
#include <vector>

#if __has_include(<mysql.h>)
    #include <mysql.h>
#else
    #include <mysql/mysql.h>
#endif

namespace shiro {

    class database {
    private:
        MYSQL connection;

        std::string address;
        unsigned int port;
        std::string db;

        std::string username;
        std::string password;

    public:
        database(const std::string &address, unsigned int port, const std::string &db, const std::string &username, const std::string &password);

        void connect();
        void disconnect();

        void setup();

        void update(const std::string &query_str);
        std::vector<MYSQL_ROW> query(const std::string &query_str);

        bool is_connected();
        MYSQL *get_connection();

    };

}

#endif //SHIRO_DATABASE_HH
