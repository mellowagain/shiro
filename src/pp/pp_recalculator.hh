/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2020 Marc3842h, czapek
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
