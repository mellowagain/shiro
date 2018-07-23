#ifndef SHIRO_SHIRO_HH
#define SHIRO_SHIRO_HH

#if defined(_WIN32)
    #define NOMINMAX
#endif

#include <chrono>
#include <ctime>
#include <memory>

#include "database/database.hh"
#include "thirdparty/taskscheduler.hh"

namespace shiro {

    extern std::shared_ptr<database> db_connection;

    using namespace std::chrono_literals;
    static tsc::TaskScheduler scheduler;

    static std::time_t start_time = std::time(nullptr);

    int init(int argc, char **argv);

    void destroy();

}

#endif //SHIRO_SHIRO_HH
