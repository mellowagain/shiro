#include "../thirdparty/cpptoml.hh"
#include "../thirdparty/loguru.hh"
#include "cli_args.hh"
#include "score_submission_file.hh"

// From the same creators as country_ids.cc
// I'm presenting you
// The sequel nobody asked for
// score_submission_file.cc

// I'm sorry.

static std::shared_ptr<cpptoml::table> config_file = nullptr;

bool shiro::config::score_submission::save_failed_scores = true;
bool shiro::config::score_submission::save_unranked_scores = false;

bool shiro::config::score_submission::restrict_notepad_hack = true;
bool shiro::config::score_submission::restrict_impossible_mods = true;
bool shiro::config::score_submission::restrict_no_replay = true;
bool shiro::config::score_submission::restrict_mismatching_client_version = true;
bool shiro::config::score_submission::restrict_impossible_combo = true;
bool shiro::config::score_submission::restrict_negative_values = true;
bool shiro::config::score_submission::restrict_mismatching_username = true;

int16_t shiro::config::score_submission::auto_restrict_pp_std = 1000;
int16_t shiro::config::score_submission::auto_restrict_pp_taiko = 1000;
int16_t shiro::config::score_submission::auto_restrict_pp_catch = 2000;
int16_t shiro::config::score_submission::auto_restrict_pp_mania = 3000;

bool shiro::config::score_submission::std_ranked = true;
bool shiro::config::score_submission::taiko_ranked = true;
bool shiro::config::score_submission::catch_ranked = true;
bool shiro::config::score_submission::mania_ranked = true;

bool shiro::config::score_submission::nomod_ranked = true;
bool shiro::config::score_submission::no_fail_ranked = true;
bool shiro::config::score_submission::easy_ranked = true;
bool shiro::config::score_submission::touch_device_ranked = true;
bool shiro::config::score_submission::hidden_ranked = true;
bool shiro::config::score_submission::hard_rock_ranked = true;
bool shiro::config::score_submission::sudden_death_ranked = true;
bool shiro::config::score_submission::double_time_ranked = true;
bool shiro::config::score_submission::relax_ranked = false;
bool shiro::config::score_submission::half_time_ranked = true;
bool shiro::config::score_submission::nightcore_ranked = true;
bool shiro::config::score_submission::flashlight_ranked = true;
bool shiro::config::score_submission::auto_play_ranked = false;
bool shiro::config::score_submission::spun_out_ranked = true;
bool shiro::config::score_submission::auto_pilot_ranked = false;
bool shiro::config::score_submission::perfect_ranked = true;
bool shiro::config::score_submission::fade_in_ranked = true;
bool shiro::config::score_submission::random_ranked = false;
bool shiro::config::score_submission::cinema_ranked = false;
bool shiro::config::score_submission::target_ranked = false;
bool shiro::config::score_submission::score_v2_ranked = false;

bool shiro::config::score_submission::key_1_ranked = false;
bool shiro::config::score_submission::key_2_ranked = false;
bool shiro::config::score_submission::key_3_ranked = false;
bool shiro::config::score_submission::key_4_ranked = true;
bool shiro::config::score_submission::key_5_ranked = true;
bool shiro::config::score_submission::key_6_ranked = true;
bool shiro::config::score_submission::key_7_ranked = true;
bool shiro::config::score_submission::key_8_ranked = true;
bool shiro::config::score_submission::key_9_ranked = false;
bool shiro::config::score_submission::key_coop_ranked = false;

void shiro::config::score_submission::parse() {
    if (config_file != nullptr)
        LOG_S(INFO) << "Re-parsing score_submission.toml file...";

    try {
        config_file = cpptoml::parse_file("score_submission.toml");
    } catch (const cpptoml::parse_exception &ex) {
        LOG_S(FATAL) << "Failed to parse score_submission.toml file: " << ex.what() << ".";
    }

    save_failed_scores = config_file->get_qualified_as<bool>("save_failed_scores").value_or(true);
    save_unranked_scores = config_file->get_qualified_as<bool>("save_unranked_scores").value_or(false);

    // Anti-cheat
    restrict_notepad_hack = config_file->get_qualified_as<bool>("anti_cheat.restrict_notepad_hack").value_or(true);
    restrict_impossible_mods = config_file->get_qualified_as<bool>("anti_cheat.restrict_impossible_mods").value_or(true);
    restrict_no_replay = config_file->get_qualified_as<bool>("anti_cheat.restrict_no_replay").value_or(true);
    restrict_mismatching_client_version = config_file->get_qualified_as<bool>("anti_cheat.restrict_mismatching_client_version").value_or(true);
    restrict_impossible_combo = config_file->get_qualified_as<bool>("anti_cheat.restrict_impossible_combo").value_or(true);
    restrict_negative_values = config_file->get_qualified_as<bool>("anti_cheat.restrict_negative_values").value_or(true);
    restrict_mismatching_username = config_file->get_qualified_as<bool>("anti_cheat.restrict_mismatching_username").value_or(true);

    auto_restrict_pp_std = config_file->get_qualified_as<int16_t>("anti_cheat.auto_restrict_pp_std").value_or(-1);
    auto_restrict_pp_taiko = config_file->get_qualified_as<int16_t>("anti_cheat.auto_restrict_pp_taiko").value_or(-1);
    auto_restrict_pp_catch = config_file->get_qualified_as<int16_t>("anti_cheat.auto_restrict_pp_catch").value_or(-1);
    auto_restrict_pp_mania = config_file->get_qualified_as<int16_t>("anti_cheat.auto_restrict_pp_mania").value_or(-1);

    // Ranked *modes*
    std_ranked = config_file->get_qualified_as<bool>("ranked_modes.std").value_or(true);
    taiko_ranked = config_file->get_qualified_as<bool>("ranked_modes.taiko").value_or(true);
    catch_ranked = config_file->get_qualified_as<bool>("ranked_modes.catch").value_or(true);
    mania_ranked = config_file->get_qualified_as<bool>("ranked_modes.mania").value_or(true);

    // Ranked *mods*
    nomod_ranked = config_file->get_qualified_as<bool>("ranked_mods.none").value_or(true);
    no_fail_ranked = config_file->get_qualified_as<bool>("ranked_mods.no_fail").value_or(true);
    easy_ranked = config_file->get_qualified_as<bool>("ranked_mods.easy").value_or(true);
    touch_device_ranked = config_file->get_qualified_as<bool>("ranked_mods.touch_device").value_or(true);
    hidden_ranked = config_file->get_qualified_as<bool>("ranked_mods.hidden").value_or(true);
    hard_rock_ranked = config_file->get_qualified_as<bool>("ranked_mods.hard_rock").value_or(true);
    sudden_death_ranked = config_file->get_qualified_as<bool>("ranked_mods.sudden_death").value_or(true);
    double_time_ranked = config_file->get_qualified_as<bool>("ranked_mods.double_time").value_or(true);
    relax_ranked = config_file->get_qualified_as<bool>("ranked_mods.relax").value_or(false);
    half_time_ranked = config_file->get_qualified_as<bool>("ranked_mods.half_time").value_or(true);
    nightcore_ranked = config_file->get_qualified_as<bool>("ranked_mods.nightcore").value_or(true);
    flashlight_ranked = config_file->get_qualified_as<bool>("ranked_mods.flashlight").value_or(true);
    auto_play_ranked = config_file->get_qualified_as<bool>("ranked_mods.auto_play").value_or(false);
    spun_out_ranked = config_file->get_qualified_as<bool>("ranked_mods.spun_out").value_or(true);
    auto_pilot_ranked = config_file->get_qualified_as<bool>("ranked_mods.auto_pilot").value_or(false);
    perfect_ranked = config_file->get_qualified_as<bool>("ranked_mods.perfect").value_or(true);
    fade_in_ranked = config_file->get_qualified_as<bool>("ranked_mods.fade_in").value_or(true);
    random_ranked = config_file->get_qualified_as<bool>("ranked_mods.random").value_or(false);
    cinema_ranked = config_file->get_qualified_as<bool>("ranked_mods.cinema").value_or(false);
    target_ranked = config_file->get_qualified_as<bool>("ranked_mods.target").value_or(false);
    score_v2_ranked = config_file->get_qualified_as<bool>("ranked_mods.score_v2").value_or(false);

    // Ranked *keys*
    key_1_ranked = config_file->get_qualified_as<bool>("ranked_keys.key_1").value_or(false);
    key_2_ranked = config_file->get_qualified_as<bool>("ranked_keys.key_2").value_or(false);
    key_3_ranked = config_file->get_qualified_as<bool>("ranked_keys.key_3").value_or(false);
    key_4_ranked = config_file->get_qualified_as<bool>("ranked_keys.key_4").value_or(true);
    key_5_ranked = config_file->get_qualified_as<bool>("ranked_keys.key_5").value_or(true);
    key_6_ranked = config_file->get_qualified_as<bool>("ranked_keys.key_6").value_or(true);
    key_7_ranked = config_file->get_qualified_as<bool>("ranked_keys.key_7").value_or(true);
    key_8_ranked = config_file->get_qualified_as<bool>("ranked_keys.key_8").value_or(true);
    key_9_ranked = config_file->get_qualified_as<bool>("ranked_keys.key_9").value_or(false);
    key_coop_ranked = config_file->get_qualified_as<bool>("ranked_keys.key_coop").value_or(false);

    LOG_S(INFO) << "Successfully parsed score_submission.toml.";

    // Most of these values here are boolean
    // Passing booleans as CLI arguments is a limitation of the CLI library
    // Passing them results in getting them set to true
    // Not passing them results in getting them set to false
    // To not overwrite anything, we don't add support for CLI yet.
    // Needs fixing in the future.
}
