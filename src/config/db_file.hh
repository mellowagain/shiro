#ifndef SHIRO_DB_FILE_HH
#define SHIRO_DB_FILE_HH

#include <string>

#include "../thirdparty/cpptoml.hh"

namespace shiro::config::database {

    static inline std::string address = "127.0.0.1";
    static inline unsigned int port = 3306;
    static inline std::string database = "shiro";
    static inline std::string username = "root";
    static inline std::string password = "hunter2";

    void parse();

}

#endif //SHIRO_DB_FILE_HH
