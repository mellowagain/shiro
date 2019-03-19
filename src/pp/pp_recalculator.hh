#ifndef SHIRO_PP_RECALCULATOR_HH
#define SHIRO_PP_RECALCULATOR_HH

#include <cstdint>
#include <thread>
#include <vector>
#include <shared_mutex>

#include "../utils/play_mode.hh"

namespace shiro::pp::recalculator {

    extern bool running;
    extern size_t running_threads;
    extern std::shared_timed_mutex mutex;

    void begin(utils::play_mode mode, uint32_t threads = std::thread::hardware_concurrency());

    void end(utils::play_mode mode);

    bool in_progess();

    // Spawn workers
    void recalculate(utils::play_mode mode, std::vector<int32_t> users);

}

#endif //SHIRO_PP_RECALCULATOR_HH
