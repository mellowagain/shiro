#ifndef SHIRO_DB_FILE_HH
#define SHIRO_DB_FILE_HH

#include <string>

namespace shiro::config::database {

    extern std::string address;
    extern uint32_t port;
    extern std::string database;
    extern std::string username;
    extern std::string password;

    void parse();

}

#endif //SHIRO_DB_FILE_HH
