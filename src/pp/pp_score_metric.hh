/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018 Marc3842h, czapek
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

#ifndef SHIRO_PP_SCORE_METRIC_HH
#define SHIRO_PP_SCORE_METRIC_HH

#include "../beatmaps/beatmap.hh"
#include "../scores/score.hh"

namespace shiro::pp {

    float calculate(beatmaps::beatmap beatmap, scores::score score);

    float calculate_std(beatmaps::beatmap beatmap, scores::score score);

    float calculate_taiko(beatmaps::beatmap beatmap, scores::score score);

    float calculate_mania(beatmaps::beatmap beatmap, scores::score score);

    float calculate_ctb(beatmaps::beatmap beatmap, scores::score score);

}

#endif //SHIRO_PP_SCORE_METRIC_HH