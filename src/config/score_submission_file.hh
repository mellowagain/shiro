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

#ifndef SHIRO_SCORE_SUBMISSION_FILE_HH
#define SHIRO_SCORE_SUBMISSION_FILE_HH

#include <cstdint>
#include <string>

namespace shiro::config::score_submission {

    extern bool save_failed_scores;
    extern bool save_unranked_scores;
    extern std::string overwrite_factor;

    extern bool restrict_notepad_hack;
    extern bool restrict_impossible_mods;
    extern bool restrict_no_replay;
    extern bool restrict_mismatching_client_version;
    extern bool restrict_impossible_combo;
    extern bool restrict_negative_values;
    extern bool restrict_mismatching_username;

    extern int16_t auto_restrict_pp_std;
    extern int16_t auto_restrict_pp_taiko;
    extern int16_t auto_restrict_pp_catch;
    extern int16_t auto_restrict_pp_mania;

    extern bool std_ranked;
    extern bool taiko_ranked;
    extern bool catch_ranked;
    extern bool mania_ranked;

    extern bool nomod_ranked;
    extern bool no_fail_ranked;
    extern bool easy_ranked;
    extern bool touch_device_ranked;
    extern bool hidden_ranked;
    extern bool hard_rock_ranked;
    extern bool sudden_death_ranked;
    extern bool double_time_ranked;
    extern bool relax_ranked;
    extern bool half_time_ranked;
    extern bool nightcore_ranked;
    extern bool flashlight_ranked;
    extern bool auto_play_ranked;
    extern bool spun_out_ranked;
    extern bool auto_pilot_ranked;
    extern bool perfect_ranked;
    extern bool fade_in_ranked;
    extern bool random_ranked;
    extern bool cinema_ranked;
    extern bool target_ranked;
    extern bool score_v2_ranked;

    extern bool key_1_ranked;
    extern bool key_2_ranked;
    extern bool key_3_ranked;
    extern bool key_4_ranked;
    extern bool key_5_ranked;
    extern bool key_6_ranked;
    extern bool key_7_ranked;
    extern bool key_8_ranked;
    extern bool key_9_ranked;
    extern bool key_coop_ranked;

    void parse();

}

#endif //SHIRO_SCORE_SUBMISSION_FILE_HH
