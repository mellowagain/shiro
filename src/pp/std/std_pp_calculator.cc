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

#include <cmath>
#include <regex>

#include "../../beatmaps/beatmap_difficulty.hh"
#include "../../utils/mods.hh"
#include "std_pp_calculator.hh"

shiro::pp::std_score::std_score(const shiro::beatmaps::beatmap &beatmap, const shiro::scores::score &score)
    : beatmap(beatmap)
    , score(score) {
    compute_aim_value();
    compute_speed_value();
    compute_acc_value();

    compute_total_value();
}

float shiro::pp::std_score::get_total_value() {
    return this->total_value;
}

float shiro::pp::std_score::get_accuracy() {
    if (this->get_total_hits() == 0)
        return 0.0f;

    return std::clamp(
            static_cast<float>(
                this->score._50_count * 50 +
                this->score._100_count * 100 +
                this->score._300_count * 300 /
                (get_total_hits() * 300)
            ),
            0.0f, 1.0f
    );
}

int32_t shiro::pp::std_score::get_total_hits() {
    return this->score._50_count + this->score._100_count + this->score._300_count + this->score.miss_count;
}

int32_t shiro::pp::std_score::get_total_successful_hits() {
    return this->score._50_count + this->score._100_count + this->score._300_count;
}

void shiro::pp::std_score::compute_total_value() {
    // Custom multipliers for NoFail and SpunOut.
    float multiplier = 1.12f; // This is being adjusted to keep the final pp value scaled around what it used to be when changing things

    if (this->score.mods & (int32_t) utils::mods::no_fail)
        multiplier *= 0.90f;

    if (this->score.mods & (int32_t) utils::mods::spun_out)
        multiplier *= 0.95f;

    this->total_value = std::pow(
            std::pow(this->aim_value, 1.1f) +
            std::pow(this->speed_value, 1.1f) +
            std::pow(this->acc_value, 1.1f), 1.0f / 1.1f
    ) * multiplier;
}

void shiro::pp::std_score::compute_aim_value() {
    float raw_aim = beatmaps::difficulty::get_aim(this->beatmap, this->score);

    if (this->score.mods & (uint32_t) utils::mods::touch_device)
        raw_aim = std::pow(raw_aim, 0.8f);

    this->aim_value = std::pow(5.0f * std::max(1.0f, raw_aim / 0.0675f) - 4.0f, 3.0f) / 100000.0f;

    int32_t total_hits_amount = this->get_total_hits();

    // Longer maps are worth more
    float length_bonus = 0.95f + 0.4f * std::min(1.0f, static_cast<float>(total_hits_amount) / 2000.0f) +
                      (total_hits_amount > 2000 ? std::log10(static_cast<float>(total_hits_amount) / 2000.0f) * 0.5f : 0.0f);

    this->aim_value *= length_bonus;

    // Penalize misses exponentially. This mainly fixes tag4 maps and the likes until a per-hitobject solution is available
    this->aim_value *= std::pow(0.97f, this->score.miss_count);

    // Combo scaling
    float max_combo = this->beatmap.max_combo;

    if (max_combo > 0)
        this->aim_value *= std::min(static_cast<float>(std::pow(this->score.max_combo, 0.8f) / std::pow(max_combo, 0.8f)), 1.0f);

    float approach_rate = beatmaps::difficulty::get_ar(this->beatmap, this->score);
    float approach_rate_factor = 1.0f;

    if (approach_rate > 10.33f) {
        approach_rate_factor += 0.3f * (approach_rate - 10.33f);
    } else if (approach_rate < 8.0f) {
        approach_rate_factor += 0.01f * (8.0f - approach_rate);
    }

    this->aim_value *= approach_rate_factor;

    // We want to give more reward for lower AR when it comes to aim and HD. This nerfs high AR and buffs lower AR.
    if (this->score.mods & (int32_t) utils::mods::hidden)
        this->aim_value *= 1.0f + 0.04f * (12.0f - approach_rate);

    if (this->score.mods & (int32_t) utils::mods::flashlight)
        // Apply object-based bonus for flashlight.
        this->aim_value *= 1.0f + 0.35f * std::min(1.0f, static_cast<float>(total_hits_amount) / 200.0f) +
                           (total_hits_amount > 200 ?
                            0.3f * std::min(1.0f, static_cast<float>(total_hits_amount - 200) / 300.0f) +
                            (total_hits_amount > 500 ? static_cast<float>(total_hits_amount - 500) / 1200.0f : 0.0f)
                                                    : 0.0f);

    // Scale the aim value with accuracy _slightly_
    this->aim_value *= 0.5f + get_accuracy() / 2.0f;
    // It is important to also consider accuracy difficulty when doing that
    this->aim_value *= 0.98f + (std::pow(beatmaps::difficulty::get_od(this->beatmap, this->score), 2) / 2500);
}

void shiro::pp::std_score::compute_speed_value() {
    this->speed_value = std::pow(5.0f * std::max(
            1.0f,
            beatmaps::difficulty::get_speed(this->beatmap, this->score) / 0.0675f
    ) - 4.0f, 3.0f) / 100000.0f;

    int32_t total_hits_amount = get_total_hits();

    float approach_rate = beatmaps::difficulty::get_ar(this->beatmap, this->score);
    float approach_rate_factor = 1.0f;

    if (approach_rate > 10.33f)
        approach_rate_factor += 0.3f * (approach_rate - 10.33f);

    this->speed_value *= approach_rate_factor;

    // Longer maps are worth more
    this->speed_value *=
            0.95f + 0.4f * std::min(1.0f, static_cast<float>(total_hits_amount) / 2000.0f) +
            (total_hits_amount > 2000 ? std::log10(static_cast<float>(total_hits_amount) / 2000.0f) * 0.5f : 0.0f);

    // Penalize misses exponentially. This mainly fixes tag4 maps and the likes until a per-hit object solution is available
    this->speed_value *= std::pow(0.97f, this->score.miss_count);

    // Combo scaling
    float max_combo = this->beatmap.max_combo;

    if (max_combo > 0)
        this->speed_value *= std::min(static_cast<float>(std::pow(this->score.max_combo, 0.8f) / std::pow(max_combo, 0.8f)), 1.0f);

    // We want to give more reward for lower AR when it comes to speed and HD. This nerfs high AR and buffs lower AR.
    if (this->score.mods & (int32_t) utils::mods::hidden)
        this->speed_value *= 1.0f + 0.04f * (12.0f - approach_rate);

    // Scale the speed value with accuracy _slightly_
    this->speed_value *= 0.02f + get_accuracy();
    // It is important to also consider accuracy difficulty when doing that
    this->speed_value *= 0.96f + (std::pow(beatmaps::difficulty::get_od(this->beatmap, this->score), 2) / 1600);
}

void shiro::pp::std_score::compute_acc_value() {
    // This percentage only considers HitCircles of any value - in this part of the calculation we focus on hitting the timing hit window
    float better_acc_percentage;

    int32_t high_acc_object_amount = 0;

    if (this->score.mods & (int32_t) utils::mods::score_v2) {
        high_acc_object_amount = get_total_hits();
        better_acc_percentage = get_accuracy();
    } else { // Either ScoreV1 or some unknown value. Let's default to previous behavior.
        // Might be wrong
        high_acc_object_amount = this->score._50_count + this->score._100_count + this->score._300_count + this->score.miss_count;

        if (high_acc_object_amount > 0) {
            better_acc_percentage = static_cast<float>(
                    (this->score._300_count - (get_total_hits() - high_acc_object_amount)) * 6 +
                    this->score._100_count * 2 + this->score._50_count
            ) / (high_acc_object_amount * 6);
        } else {
            better_acc_percentage = 0;
        }

        // It is possible to reach a negative accuracy with this formula. Cap it at zero - zero points
        if (better_acc_percentage < 0)
            better_acc_percentage = 0;
    }

    // Lots of arbitrary values from testing.
    // Considering to use derivation from perfect accuracy in a probabilistic manner - assume normal distribution
    this->acc_value = std::pow(
            1.52163f,
            beatmaps::difficulty::get_od(this->beatmap, this->score)
    ) * std::pow(better_acc_percentage, 24) * 2.83f;

    // Bonus for many hit circles - it's harder to keep good accuracy up for longer
    this->acc_value *= std::min(1.15f, std::pow(high_acc_object_amount / 1000.0f, 0.3f));

    if (this->score.mods & (int32_t) utils::mods::hidden)
        this->acc_value *= 1.08f;

    if (this->score.mods & (int32_t) utils::mods::flashlight)
        this->acc_value *= 1.02f;
}
