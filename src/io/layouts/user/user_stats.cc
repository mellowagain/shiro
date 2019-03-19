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
#include <limits>

#include "../../../pp/pp_recalculator.hh"
#include "../../../scores/score.hh"
#include "../../../scores/score_helper.hh"
#include "../../../utils/osu_string.hh"
#include "../../../utils/play_mode.hh"
#include "user_stats.hh"

void shiro::io::layouts::user_stats::recalculate_accuracy() {
    std::vector<scores::score> scores = scores::helper::fetch_top100_user((utils::play_mode) this->play_mode, this->user_id);
    float accuracy = 0.0f;

    for (const scores::score &score : scores) {
        accuracy += score.accuracy;
    }

    this->accuracy = std::clamp(accuracy / scores.size(), 0.0f, 100.0f);
}

void shiro::io::layouts::user_stats::recalculate_pp() {
    // Global pp recalculation is currently in progress.
    if (pp::recalculator::in_progess())
        return;

    std::vector<scores::score> scores = scores::helper::fetch_top100_user((utils::play_mode) this->play_mode, this->user_id);
    float pp = 0; // Here it is a float to keep decimal points, round it when setting final pp value

    for (size_t i = 0; i < scores.size(); i++) {
        scores::score score = scores.at(i);

        pp += (score.pp * std::pow(0.95, i));
    }

    this->pp = std::clamp(static_cast<int16_t>(pp), (int16_t) 0, std::numeric_limits<int16_t>::max());
}

shiro::io::buffer shiro::io::layouts::user_stats::marshal() {
    std::string status_text = utils::osu_string(this->activity_desc);
    std::string beatmap_checksum = utils::osu_string(this->beatmap_checksum);

    buffer buf;

    buf.write<int32_t>(this->user_id);

    buf.write<uint8_t>(this->activity);
    buf.write_string(status_text);

    buf.write_string(beatmap_checksum);

    buf.write<uint32_t>(this->current_mods);
    buf.write<uint8_t>(this->play_mode);

    buf.write<int32_t>(this->beatmap_id);

    buf.write<uint64_t>(this->ranked_score);

    buf.write<float>(this->accuracy / 100);
    buf.write<int32_t>(this->play_count);
    buf.write<uint64_t>(this->total_score);

    buf.write<int32_t>(this->rank);
    buf.write<int16_t>(this->pp);

    return buf;
}

void shiro::io::layouts::user_stats::unmarshal(shiro::io::buffer &buffer) {
    this->user_id = buffer.read<int32_t>();

    this->activity = buffer.read<uint8_t>();
    this->activity_desc = buffer.read_string();

    this->beatmap_checksum = buffer.read_string();

    this->current_mods = buffer.read<uint32_t>();
    this->play_mode = buffer.read<uint8_t>();

    this->beatmap_id = buffer.read<int32_t>();

    this->ranked_score = buffer.read<uint64_t>();

    this->accuracy = buffer.read<float>() * 100;
    this->play_count = buffer.read<int32_t>();
    this->total_score = buffer.read<uint64_t>();

    this->rank = buffer.read<int32_t>();
    this->pp = buffer.read<int16_t>();
}

int32_t shiro::io::layouts::user_stats::get_size() {
    return (int32_t) this->marshal().get_size();
}
