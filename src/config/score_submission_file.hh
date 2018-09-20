#ifndef SHIRO_SCORE_SUBMISSION_FILE_HH
#define SHIRO_SCORE_SUBMISSION_FILE_HH

#include <cstdint>

namespace shiro::config::score_submission {

    extern bool save_failed_scores;
    extern bool save_unranked_scores;

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
