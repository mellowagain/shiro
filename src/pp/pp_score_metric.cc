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

#include <utility>

#include "wrapper/oppai_wrapper.hh"
#include "pp_score_metric.hh"

float shiro::pp::calculate(shiro::beatmaps::beatmap beatmap, shiro::scores::score score) {
    switch ((utils::play_mode) score.play_mode) {
        case utils::play_mode::standard:
            return calculate_std(std::move(beatmap), std::move(score));
        case utils::play_mode::taiko:
            return calculate_taiko(std::move(beatmap), std::move(score));
        case utils::play_mode::fruits:
            return calculate_ctb(std::move(beatmap), std::move(score));
        case utils::play_mode::mania:
            return calculate_mania(std::move(beatmap), std::move(score));
    }

    return 0.0f;
}

float shiro::pp::calculate_std(shiro::beatmaps::beatmap beatmap, shiro::scores::score score) {
    oppai_wrapper wrapper(std::move(beatmap), std::move(score));

    return wrapper.calculate();
}

// For now this just uses oppai-ng and thus does not award correct pp for converted maps
float shiro::pp::calculate_taiko(shiro::beatmaps::beatmap beatmap, shiro::scores::score score) {
    return calculate_std(std::move(beatmap), std::move(score));
}

// TODO: Implement pp calculation for osu!mania
float shiro::pp::calculate_mania(shiro::beatmaps::beatmap beatmap, shiro::scores::score score) {
    return 0.0f;
}

// TODO: Implement pp calculation for osu!catch
float shiro::pp::calculate_ctb(shiro::beatmaps::beatmap beatmap, shiro::scores::score score) {
    return 0.0f;
}
