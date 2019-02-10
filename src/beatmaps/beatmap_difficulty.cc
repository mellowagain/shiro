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

#include <algorithm>
#include <cmath>
#include <memory>

#include "../utils/math_utils.hh"
#include "../utils/mods.hh"
#include "beatmap_difficulty.hh"
#include "beatmap_parser.hh"

float shiro::beatmaps::difficulty::get_speed(const shiro::beatmaps::beatmap &map, const scores::score &score) {
    if (!(score.mods & utils::map_changing_mods))
        return 1.0f;

    float speed = 1.0f;

    // Includes Nightcore as it is set additionally
    if (score.mods & (int32_t) utils::mods::double_time)
        speed = 1.5f;

    if (score.mods & (int32_t) utils::mods::half_time)
        speed *= 0.75f;

    return speed;
}

float shiro::beatmaps::difficulty::get_aim(const shiro::beatmaps::beatmap &map, const scores::score &score) {
    std::unique_ptr<beatmaps::parser> parser = std::make_unique<beatmaps::parser>(map.beatmap_id);
    parser->parse();

    std::vector<double> strains;
    double strain_step = 400.0;
    double interval_end = strain_step;
    double max_strain = 0.0;

    for (size_t i = 0; i < parser->objects.size(); i++) {
        beatmaps::hit_object object = parser->objects.at(i);

        if (i > 0) {
            beatmaps::hit_object previous = parser->objects.at(i - 1);
            double time_elapsed = object.time - previous.time;
            double decay = std::pow(0.15, time_elapsed / 1000.0);
            double value = 0.0;

            object.delta_time = time_elapsed;

            if (!(object.type & beatmaps::spinner_type)) {
                double distance = (object.position.first - previous.position.first) + (object.position.second - previous.position.second);
                double strain_time = std::max(time_elapsed, 50.0);
                double previous_strain_time = std::max(previous.delta_time, 50.0);
                double angle_bonus;
                double result = 0.0;

                if (!std::isnan(object.angle) && object.angle > (utils::math::d_pi() * 3)) {
                    angle_bonus = std::sqrt(
                            std::max(previous.distance - 90.0, 0.0) *
                            std::pow(std::sin(object.angle - (utils::math::d_pi() * 3)), 2.0) *
                            std::max(object.distance - 90.0, 0.0)
                    );
                    result = 1.5 * std::pow(std::max(0.0, angle_bonus), 0.99) / std::max(107.0, previous_strain_time);
                }

                double weighted_distance = std::pow(distance, 0.99);
                value = std::max(result + weighted_distance / std::max(107.0, strain_time), weighted_distance / strain_step);
                value *= 26.25;
            }

            object.strains.at(1) = previous.strains.at(1) * decay + value;
        }

        while (object.time > interval_end) {
            strains.emplace_back(max_strain);

            if (i > 0) {
                beatmaps::hit_object previous = parser->objects.at(i - 1);

                double decay = std::pow(0.15, (interval_end - previous.time) / 100.0);
                max_strain = previous.strains.at(2) * decay;
            } else {
                max_strain = 0.0;
            }

            interval_end += strain_step;
        }

        max_strain = std::max(max_strain, object.strains.at(2));
    }

    std::reverse(strains.begin(), strains.end());

    double weight = 1.0;
    double difficulty = 0.0;

    for (double strain : strains) {
        difficulty += strain * weight;
        weight *= 0.9;
    }

    return static_cast<float>(difficulty);
}

float shiro::beatmaps::difficulty::get_od(const shiro::beatmaps::beatmap &map, const scores::score &score) {
    float multiplier = 1.0f;

    if (score.mods & (int32_t) utils::mods::hard_rock)
        multiplier *= 1.4f;

    if (score.mods & (int32_t) utils::mods::half_time)
        multiplier *= 0.5f;

    float od = map.od * multiplier;
    float od_ms = 79.5f - std::ceil((79.5f - 19.5f) * od);
    od_ms = std::min(79.5f, std::max(19.5f, od_ms));
    od_ms /= get_speed(map, score); // TODO: Cache result of get_speed for beatmap and score
    od = (79.5f - od_ms) / ((79.5f - 19.5f) / 10.0f);

    return od;
}

float shiro::beatmaps::difficulty::get_ar(const shiro::beatmaps::beatmap &map, const scores::score &score) {
    float multiplier = 1.0f;

    if (score.mods & (int32_t) utils::mods::hard_rock)
        multiplier *= 1.4f;

    if (score.mods & (int32_t) utils::mods::half_time)
        multiplier *= 0.5f;

    float ar = map.ar * multiplier;
    float ar_ms = ar < 5.0f ? 1800.0f - ((1800.0f - 1200.0f) / 5.0f) * ar : 1200.0f - ((1200.0f - 450.0f) / 5.0f) * (ar - 5.0f);
    ar_ms = std::min(1800.0f, std::max(450.0f, ar_ms));
    ar_ms /= get_speed(map, score); // TODO: Cache result of get_speed for beatmap and score
    ar = ar_ms > 1200.0f ? (1800.0f - ar_ms) / ((1800.0f - 1200.0f) / 5.0f) : 5.0f + (1200.0f - ar_ms) / ((1200.0f - 450.0f) / 5.0f);

    return ar;
}
