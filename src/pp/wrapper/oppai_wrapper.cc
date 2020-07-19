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

#define OPPAI_IMPLEMENTATION

#include "../../beatmaps/beatmap_helper.hh"
#include "../../thirdparty/oppai.hh"
#include "../../utils/mods.hh"
#include "oppai_wrapper.hh"

shiro::pp::oppai_wrapper::oppai_wrapper(shiro::beatmaps::beatmap beatmap, shiro::scores::score score) {
    std::optional<std::string> beatmap_file = beatmaps::helper::get_location(beatmap.beatmap_id);

    if (!beatmap_file.has_value())
        return;

    this->ez = ezpp_new();

    // Call ezpp_dup instead of ezpp so it strdup's the const char* into an char*
    ezpp_dup(this->ez, beatmap_file.value().c_str());

    ezpp_set_mode(this->ez, score.play_mode);
    ezpp_set_mods(this->ez, score.mods);
    ezpp_set_combo(this->ez, score.max_combo);

    this->ez->n300 = score._300_count;
    this->ez->n100 = score._100_count;
    this->ez->n50 = score._50_count;
    ezpp_set_nmiss(this->ez, score.miss_count);

    // Enable auto calc now so the next call to ezpp_set below will calculate the whole beatmap difficulty
    ezpp_set_autocalc(this->ez, true);

    ezpp_set_score_version(this->ez, (score.mods & (int32_t) utils::mods::score_v2) ? 2 : 1);
}

shiro::pp::oppai_wrapper::~oppai_wrapper() {
    if (this->ez == nullptr)
        return;

    ezpp_free(this->ez);
}

float shiro::pp::oppai_wrapper::calculate() {
    if (this->ez == nullptr)
        return 0.0f;

    return ezpp_pp(this->ez);
}
