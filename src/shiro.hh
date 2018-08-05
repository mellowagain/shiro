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
    extern tsc::TaskScheduler scheduler;

    extern std::time_t start_time;

    int init(int argc, char **argv);

    void destroy();

}

#endif //SHIRO_SHIRO_HH
