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

#ifndef SHIRO_BEATMAP_HIT_OBJECT_HH
#define SHIRO_BEATMAP_HIT_OBJECT_HH

#include <cstdint>
#include <array>

namespace shiro::beatmaps {

    class hit_object {
    public:
        double time = 0.0;
        int32_t type = 0;

        std::array<double, 2> strains = { 0.0, 0.0 };

        std::pair<float, float> position;

        double angle = 0.0;
        bool single = false;

        double delta_time = 0.0;
        double distance = 0.0;

    };

    class slider : public hit_object {
    public:
        double distance = 0.0;
        int32_t repetitions = 0; // Thank you for making kick sliders

    };
    
}

#endif //SHIRO_BEATMAP_HIT_OBJECT_HH
