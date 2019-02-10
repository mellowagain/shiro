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

#ifndef SHIRO_STD_PP_CALCULATOR_HH
#define SHIRO_STD_PP_CALCULATOR_HH

#include "../../beatmaps/beatmap.hh"
#include "../../scores/score.hh"

namespace shiro::pp {

    class std_score {
    public:
        std_score(const beatmaps::beatmap &beatmap, const scores::score &score);

        float get_total_value();
        float get_accuracy();
        int32_t get_total_hits();
        int32_t get_total_successful_hits();

    private:
        beatmaps::beatmap beatmap;
        scores::score score;

        float aim_value = 0.0f;
        float speed_value = 0.0f;
        float acc_value = 0.0f;
        float total_value = 0.0f;

        void compute_total_value();

        void compute_aim_value();
        void compute_speed_value();
        void compute_acc_value();

    };

}

#endif //SHIRO_STD_PP_CALCULATOR_HH
