#ifndef SHIRO_MODS_HH
#define SHIRO_MODS_HH

#include <cstdint>

namespace shiro::utils {

    enum class mods : int32_t {
        none = 0 << 0,
        no_fail = 1 << 0,
        easy = 1 << 1,
        touch_device = 1 << 2,
        hidden = 1 << 3,
        hard_rock = 1 << 4,
        sudden_death = 1 << 5,
        double_time = 1 << 6,
        relax = 1 << 7,
        half_time = 1 << 8,
        nightcore = 1 << 9, // only set together with DT (mods::double_time | mods::nightcore)
        flashlight = 1 << 10,
        auto_play = 1 << 11,
        spun_out = 1 << 12,
        auto_pilot = 1 << 13, // relax2
        perfect = 1 << 14, // only set together with SD (mods::sudden_death | mods::perfect)

        key_4 = 1 << 15,
        key_5 = 1 << 16,
        key_6 = 1 << 17,
        key_7 = 1 << 18,
        key_8 = 1 << 19,
        fade_in = 1 << 20,
        random = 1 << 21,
        cinema = 1 << 22,
        target = 1 << 23,
        key_9 = 1 << 24,
        key_coop = 1 << 25,
        key_1 = 1 << 26,
        key_3 = 1 << 27,
        key_2 = 1 << 28,
        score_v2 = 1 << 29,
        last_mod = 1 << 30

    };

    constexpr int32_t key_mods = (int32_t) mods::key_1 |
            (int32_t) mods::key_2 |
            (int32_t) mods::key_3 |
            (int32_t) mods::key_4 |
            (int32_t) mods::key_5 |
            (int32_t) mods::key_6 |
            (int32_t) mods::key_7 |
            (int32_t) mods::key_8 |
            (int32_t) mods::key_9 |
            (int32_t) mods::key_coop;

    constexpr int32_t free_mods = (int32_t) mods::no_fail |
            (int32_t) mods::easy |
            (int32_t) mods::hidden |
            (int32_t) mods::hard_rock |
            (int32_t) mods::sudden_death |
            (int32_t) mods::flashlight |
            (int32_t) mods::fade_in |
            (int32_t) mods::relax |
            (int32_t) mods::auto_pilot |
            (int32_t) mods::spun_out |
            key_mods;

    constexpr int32_t score_increase_mods = (int32_t) mods::hidden |
            (int32_t) mods::hard_rock |
            (int32_t) mods::double_time |
            (int32_t) mods::flashlight |
            (int32_t) mods::fade_in;

}

#endif  // SHIRO_MODS_HH
