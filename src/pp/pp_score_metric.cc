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

#define OPPAI_IMPLEMENTATION // Needs to be always defined before including oppai.hh

#include <utility>

#include "../beatmaps/beatmap_helper.hh"
#include "../thirdparty/loguru.hh"
#include "../thirdparty/oppai.hh"
#include "../utils/math_utils.hh"
#include "../utils/mods.hh"
#include "std/std_pp_calculator.hh"
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
    std_score score_calculator(beatmap, score);
    float pp = score_calculator.get_total_value();
    float fallback = fallback::calculate_std(beatmap, score);

    // Warn for now because the new calculator is experimental
    if (!utils::math::identical(pp, fallback))
        LOG_F(WARNING, "Mismatch between shiro-performance and oppai-ng: %f != %f", pp, fallback);

    // Return always fallback. If the two values are not identical, we want the value that is most likely
    // correct (which would be oppai-ng), and if they were the same it wouldn't matter which one we returned
    return fallback;
}

// For now this just uses oppai-ng and thus does not award correct pp for converted maps
float shiro::pp::calculate_taiko(shiro::beatmaps::beatmap beatmap, shiro::scores::score score) {
    return fallback::calculate_std(std::move(beatmap), std::move(score));
}

// TODO: Implement pp calculation for osu!mania
float shiro::pp::calculate_mania(shiro::beatmaps::beatmap beatmap, shiro::scores::score score) {
    return 0.0f;
}

// TODO: Implement pp calculation for osu!catch
float shiro::pp::calculate_ctb(shiro::beatmaps::beatmap beatmap, shiro::scores::score score) {
    return 0.0f;
}

[[deprecated("This method will be removed once shiro-performance is no longer experimental")]]
float shiro::pp::fallback::calculate_std(shiro::beatmaps::beatmap beatmap, shiro::scores::score score) {
    struct parser parser_state;
    struct beatmap map;

    struct diff_calc difficulty;
    struct pp_calc pp;

    struct pp_params params;

    FILE *file = beatmaps::helper::download(beatmap.beatmap_id);

    p_init(&parser_state);
    p_map(&parser_state, &map, file);

    d_init(&difficulty);
    d_calc(&difficulty, &map, score.mods);

    params.aim = difficulty.aim;
    params.speed = difficulty.speed;
    params.max_combo = beatmap.max_combo;
    params.nsliders = map.nsliders;
    params.ncircles = map.ncircles;
    params.nobjects = map.nobjects;

    params.mode = score.play_mode;
    params.mods = score.mods;
    params.combo = score.max_combo;
    params.n300 = score._300_count;
    params.n100 = score._100_count;
    params.n50 = score._50_count;
    params.nmiss = score.miss_count;
    params.score_version = (score.mods & (int32_t) utils::mods::score_v2) ? 2 : 1;

    ppv2p(&pp, &params);

    float calculated_pp = pp.total;

    d_free(&difficulty);
    p_free(&parser_state);

    std::fclose(file);

    return calculated_pp;
}
