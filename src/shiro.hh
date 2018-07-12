#ifndef SHIRO_SHIRO_HH
#define SHIRO_SHIRO_HH

#if defined(_WIN32)
    #define NOMINMAX
#endif

#include <ctime>
#include <memory>

#include "database/database.hh"

namespace shiro {

    static std::shared_ptr<database> db_connection = nullptr;

    static std::time_t start_time = std::time(nullptr);

    int init(int argc, char **argv);

    void destroy();

}

#endif //SHIRO_SHIRO_HH
