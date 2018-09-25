#include <chrono>

#include "time_utils.hh"

constexpr int64_t unix_epoch_ticks = 621355968000000000;

int64_t shiro::utils::time::get_current_time_ticks() {
    using ticks = std::chrono::duration<int64_t, std::ratio_multiply<std::ratio<100>, std::nano>>;

    std::chrono::time_point now = std::chrono::system_clock::now();
    int64_t ticks_since_epoch = std::chrono::duration_cast<ticks>(now.time_since_epoch()).count();
    ticks_since_epoch += unix_epoch_ticks;

    return ticks_since_epoch;
}
