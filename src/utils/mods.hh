/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2019 Marc3842h, czapek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SHIRO_MODS_HH
#define SHIRO_MODS_HH

#include <cstdint>

namespace shiro::utils {

    enum class mods : uint32_t {
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

    constexpr uint32_t key_mods = (uint32_t) mods::key_1 |
            (uint32_t) mods::key_2 |
            (uint32_t) mods::key_3 |
            (uint32_t) mods::key_4 |
            (uint32_t) mods::key_5 |
            (uint32_t) mods::key_6 |
            (uint32_t) mods::key_7 |
            (uint32_t) mods::key_8 |
            (uint32_t) mods::key_9 |
            (uint32_t) mods::key_coop;

    constexpr uint32_t unranked_key_mods = (uint32_t) mods::key_1 |
            (uint32_t) mods::key_2 |
            (uint32_t) mods::key_3 |
            (uint32_t) mods::key_9 |
            (uint32_t) mods::key_coop;

    constexpr uint32_t free_mods = (uint32_t) mods::no_fail |
            (uint32_t) mods::easy |
            (uint32_t) mods::hidden |
            (uint32_t) mods::hard_rock |
            (uint32_t) mods::sudden_death |
            (uint32_t) mods::flashlight |
            (uint32_t) mods::fade_in |
            (uint32_t) mods::relax |
            (uint32_t) mods::auto_pilot |
            (uint32_t) mods::spun_out |
            key_mods;

    constexpr uint32_t score_increase_mods = (uint32_t) mods::hidden |
            (uint32_t) mods::hard_rock |
            (uint32_t) mods::double_time |
            (uint32_t) mods::flashlight |
            (uint32_t) mods::fade_in;

}

#endif  // SHIRO_MODS_HH
