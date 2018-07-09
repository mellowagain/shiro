#ifndef SHIRO_DB_FILE_HH
#define SHIRO_DB_FILE_HH

#include <string>

#include "../thirdparty/cpptoml.hh"

namespace shiro::config::database {

    extern std::string address;
    extern unsigned int port;
    extern std::string database;
    extern std::string username;
    extern std::string password;

    void parse();

}

#endif //SHIRO_DB_FILE_HH
