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

#include "../beatmaps/beatmap.hh"
#include "../beatmaps/beatmap_helper.hh"
#include "../config/score_submission_file.hh"
#include "../database/tables/score_table.hh"
#include "../geoloc/country_ids.hh"
#include "../thirdparty/loguru.hh"
#include "../thirdparty/oppai.hh"
#include "../users/user_manager.hh"
#include "../users/user_punishments.hh"
#include "../utils/mods.hh"
#include "score_helper.hh"

shiro::scores::score shiro::scores::helper::fetch_top_score_user(std::string beatmap_md5sum, std::shared_ptr<shiro::users::user> user) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.beatmap_md5 == beatmap_md5sum and score_table.user_id == user->user_id));

    if (result.empty())
        return score(-1);

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = beatmap_md5sum;

        s.rank = row.ranking;
        s.total_score = row.score;
        s.max_combo = row.max_combo;
        s.pp = row.pp;

        s.accuracy = row.accuracy;
        s.mods = row.mods;

        s.fc = row.fc;
        s.passed = row.passed;

        s._300_count = row._300_count;
        s._100_count = row._100_count;
        s._50_count = row._50_count;
        s.katus_count = row.katus_count;
        s.gekis_count = row.gekis_count;
        s.miss_count = row.miss_count;

        s.play_mode = row.play_mode;
        s.time = row.time;

        if (!s.passed)
            continue;

        if (!is_ranked(s, beatmaps::beatmap()))
            continue;

        scores.emplace_back(s);
    }

    if (scores.empty())
        return score(-1);

    std::sort(scores.begin(), scores.end(), [](const score &s_left, const score &s_right) {
        return s_left.total_score > s_right.total_score;
    });

    return scores.at(0);
}

std::vector<shiro::scores::score> shiro::scores::helper::fetch_all_scores(std::string beatmap_md5sum, size_t limit) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.beatmap_md5 == beatmap_md5sum));

    if (result.empty())
        return {};

    beatmaps::beatmap map;
    map.beatmap_md5 = beatmap_md5sum;

    if (!map.fetch_db()) {
        LOG_F(ERROR, "Tried to fetch scores for beatmap hash %s without it being in database.", beatmap_md5sum.c_str());
        return {};
    }

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = beatmap_md5sum;

        s.rank = row.ranking;
        s.total_score = row.score;
        s.max_combo = row.max_combo;
        s.pp = row.pp;

        s.accuracy = row.accuracy;
        s.mods = row.mods;

        s.fc = row.fc;
        s.passed = row.passed;

        s._300_count = row._300_count;
        s._100_count = row._100_count;
        s._50_count = row._50_count;
        s.katus_count = row.katus_count;
        s.gekis_count = row.gekis_count;
        s.miss_count = row.miss_count;

        s.play_mode = row.play_mode;
        s.time = row.time;

        if (!s.passed)
            continue;

        if (!users::punishments::has_scores(s.user_id))
            continue;

        if (!is_ranked(s, beatmaps::beatmap()))
            continue;

        if (map.last_update > s.time)
            continue;

        scores.emplace_back(s);
    }

    scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const score &s) {
        for (const score &score : scores) {
            if (score.hash == s.hash)
                continue;

            if (score.user_id == s.user_id && score.total_score > s.total_score)
                return true;
        }

        return false;
    }), scores.end());

    std::sort(scores.begin(), scores.end(), [](const score &s_left, const score &s_right) {
        return s_left.total_score > s_right.total_score;
    });

    if (scores.size() > limit)
        scores.resize(limit);

    return scores;
}

std::vector<shiro::scores::score> shiro::scores::helper::fetch_country_scores(std::string beatmap_md5sum, uint8_t country, size_t limit) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.beatmap_md5 == beatmap_md5sum));

    if (result.empty())
        return {};

    beatmaps::beatmap map;
    map.beatmap_md5 = beatmap_md5sum;

    if (!map.fetch_db()) {
        LOG_F(ERROR, "Tried to fetch scores for beatmap hash %s without it being in database.", beatmap_md5sum.c_str());
        return {};
    }

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = beatmap_md5sum;

        s.rank = row.ranking;
        s.total_score = row.score;
        s.max_combo = row.max_combo;
        s.pp = row.pp;

        s.accuracy = row.accuracy;
        s.mods = row.mods;

        s.fc = row.fc;
        s.passed = row.passed;

        s._300_count = row._300_count;
        s._100_count = row._100_count;
        s._50_count = row._50_count;
        s.katus_count = row.katus_count;
        s.gekis_count = row.gekis_count;
        s.miss_count = row.miss_count;

        s.play_mode = row.play_mode;
        s.time = row.time;

        if (!s.passed)
            continue;

        if (!users::punishments::has_scores(s.user_id))
            continue;

        if (!is_ranked(s, beatmaps::beatmap()))
            continue;

        if (map.last_update > s.time)
            continue;

        scores.emplace_back(s);
    }

    scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const score &s) {
        std::shared_ptr<users::user> user = users::manager::get_user_by_id(s.user_id);

        if (user == nullptr) {
            user = std::make_shared<users::user>(s.user_id);

            if (!user->init())
                return true;
        }

        uint8_t user_country = geoloc::get_country_id(user->country);
        if (user_country != country)
            return true;

        for (const score &score : scores) {
            if (s.hash == score.hash || s.user_id != score.user_id)
                continue;

            if (score.total_score > s.total_score)
                return true;
        }

        return false;
    }), scores.end());

    std::sort(scores.begin(), scores.end(), [](const score &s_left, const score &s_right) {
        return s_left.total_score > s_right.total_score;
    });

    if (scores.size() > limit)
        scores.resize(limit);

    return scores;
}

std::vector<shiro::scores::score> shiro::scores::helper::fetch_mod_scores(std::string beatmap_md5sum, int32_t mods, size_t limit) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.beatmap_md5 == beatmap_md5sum));

    if (result.empty())
        return {};

    beatmaps::beatmap map;
    map.beatmap_md5 = beatmap_md5sum;

    if (!map.fetch_db()) {
        LOG_F(ERROR, "Tried to fetch scores for beatmap hash %s without it being in database.", beatmap_md5sum.c_str());
        return {};
    }

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = beatmap_md5sum;

        s.rank = row.ranking;
        s.total_score = row.score;
        s.max_combo = row.max_combo;
        s.pp = row.pp;

        s.accuracy = row.accuracy;
        s.mods = row.mods;

        s.fc = row.fc;
        s.passed = row.passed;

        s._300_count = row._300_count;
        s._100_count = row._100_count;
        s._50_count = row._50_count;
        s.katus_count = row.katus_count;
        s.gekis_count = row.gekis_count;
        s.miss_count = row.miss_count;

        s.play_mode = row.play_mode;
        s.time = row.time;

        if (!s.passed)
            continue;

        if (!users::punishments::has_scores(s.user_id))
            continue;

        if (!is_ranked(s, beatmaps::beatmap()))
            continue;

        if (map.last_update > s.time)
            continue;

        scores.emplace_back(s);
    }

    scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const score &s) {
        if (s.mods != mods)
            return true;

        for (const score &score : scores) {
            if (s.hash == score.hash || s.user_id != score.user_id)
                continue;

            if (score.total_score > s.total_score)
                return true;
        }

        return false;
    }), scores.end());

    std::sort(scores.begin(), scores.end(), [](const score &s_left, const score &s_right) {
        return s_left.total_score > s_right.total_score;
    });

    if (scores.size() > limit)
        scores.resize(limit);

    return scores;
}

std::vector<shiro::scores::score> shiro::scores::helper::fetch_friend_scores(std::string beatmap_md5sum, std::shared_ptr<shiro::users::user> user, size_t limit) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.beatmap_md5 == beatmap_md5sum));

    if (result.empty())
        return {};

    beatmaps::beatmap map;
    map.beatmap_md5 = beatmap_md5sum;

    if (!map.fetch_db()) {
        LOG_F(ERROR, "Tried to fetch scores for beatmap hash %s without it being in database.", beatmap_md5sum.c_str());
        return {};
    }

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = beatmap_md5sum;

        s.rank = row.ranking;
        s.total_score = row.score;
        s.max_combo = row.max_combo;
        s.pp = row.pp;

        s.accuracy = row.accuracy;
        s.mods = row.mods;

        s.fc = row.fc;
        s.passed = row.passed;

        s._300_count = row._300_count;
        s._100_count = row._100_count;
        s._50_count = row._50_count;
        s.katus_count = row.katus_count;
        s.gekis_count = row.gekis_count;
        s.miss_count = row.miss_count;

        s.play_mode = row.play_mode;
        s.time = row.time;

        if (!s.passed)
            continue;

        if (!users::punishments::has_scores(s.user_id))
            continue;

        if (!is_ranked(s, beatmaps::beatmap()))
            continue;

        if (map.last_update > s.time)
            continue;

        scores.emplace_back(s);
    }

    scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const score &s) {
        std::shared_ptr<users::user> score_user = users::manager::get_user_by_id(s.user_id);

        if (score_user == nullptr) {
            score_user = std::make_shared<users::user>(s.user_id);

            if (!score_user->init())
                return true;
        }

        if (score_user->user_id == user->user_id)
            return false;

        return std::find(user->friends.begin(), user->friends.end(), score_user->user_id) == user->friends.end();
    }), scores.end());

    scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const score &s) {
        for (const score &score : scores) {
            if (score.hash == s.hash || score.user_id != s.user_id)
                continue;

            if (score.total_score > s.total_score)
                return true;
        }

        return false;
    }), scores.end());

    std::sort(scores.begin(), scores.end(), [](const score &s_left, const score &s_right) {
        return s_left.total_score > s_right.total_score;
    });

    if (scores.size() > limit)
        scores.resize(limit);

    return scores;
}

std::vector<shiro::scores::score> shiro::scores::helper::fetch_user_scores(std::string beatmap_md5sum, std::shared_ptr<shiro::users::user> user, size_t limit) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(
            score_table.beatmap_md5 == beatmap_md5sum and score_table.user_id == user->user_id
    ).limit(limit));

    if (result.empty())
        return {};

    beatmaps::beatmap map;
    map.beatmap_md5 = beatmap_md5sum;

    if (!map.fetch_db()) {
        LOG_F(ERROR, "Tried to fetch scores for beatmap hash %s without it being in database.", beatmap_md5sum.c_str());
        return {};
    }

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = beatmap_md5sum;

        s.rank = row.ranking;
        s.total_score = row.score;
        s.max_combo = row.max_combo;
        s.pp = row.pp;

        s.accuracy = row.accuracy;
        s.mods = row.mods;

        s.fc = row.fc;
        s.passed = row.passed;

        s._300_count = row._300_count;
        s._100_count = row._100_count;
        s._50_count = row._50_count;
        s.katus_count = row.katus_count;
        s.gekis_count = row.gekis_count;
        s.miss_count = row.miss_count;

        s.play_mode = row.play_mode;
        s.time = row.time;

        if (!s.passed)
            continue;

        if (!is_ranked(s, beatmaps::beatmap()))
            continue;

        if (map.last_update > s.time)
            continue;

        scores.emplace_back(s);
    }

    std::sort(scores.begin(), scores.end(), [](const score &s_left, const score &s_right) {
        return s_left.total_score > s_right.total_score;
    });

    return scores;
}

std::vector<shiro::scores::score> shiro::scores::helper::fetch_all_user_scores(int32_t user_id, size_t limit) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.user_id == user_id).limit(limit));

    if (result.empty())
        return {};

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = row.beatmap_md5;

        s.rank = row.ranking;
        s.total_score = row.score;
        s.max_combo = row.max_combo;
        s.pp = row.pp;

        s.accuracy = row.accuracy;
        s.mods = row.mods;

        s.fc = row.fc;
        s.passed = row.passed;

        s._300_count = row._300_count;
        s._100_count = row._100_count;
        s._50_count = row._50_count;
        s.katus_count = row.katus_count;
        s.gekis_count = row.gekis_count;
        s.miss_count = row.miss_count;

        s.play_mode = row.play_mode;
        s.time = row.time;

        scores.emplace_back(s);
    }

    std::sort(scores.begin(), scores.end(), [](const score &s_left, const score &s_right) {
        return s_left.total_score > s_right.total_score;
    });

    return scores;
}

std::vector<shiro::scores::score> shiro::scores::helper::fetch_top100_user(shiro::utils::play_mode mode, int32_t user_id) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.user_id == user_id and score_table.play_mode == (uint8_t) mode));

    if (result.empty())
        return {};

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = row.beatmap_md5;

        s.rank = row.ranking;
        s.total_score = row.score;
        s.max_combo = row.max_combo;
        s.pp = row.pp;

        s.accuracy = row.accuracy;
        s.mods = row.mods;

        s.fc = row.fc;
        s.passed = row.passed;

        s._300_count = row._300_count;
        s._100_count = row._100_count;
        s._50_count = row._50_count;
        s.katus_count = row.katus_count;
        s.gekis_count = row.gekis_count;
        s.miss_count = row.miss_count;

        s.play_mode = row.play_mode;
        s.time = row.time;

        if (!s.passed)
            continue;

        if (!is_ranked(s, beatmaps::beatmap()))
            continue;

        scores.emplace_back(s);
    }

    scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const score &s) {
        beatmaps::beatmap beatmap;
        beatmap.beatmap_md5 = s.beatmap_md5;

        // How do we have a score on the beatmap without having the beatmap in the db?
        if (!beatmap.fetch_db()) {
            LOG_F(ERROR, "Found score for user %i without having beatmap hash %s in database.", user_id, s.beatmap_md5.c_str());
            return true;
        }

        if (!beatmaps::helper::has_leaderboard(beatmaps::helper::fix_beatmap_status(beatmap.ranked_status)))
            return true;

        for (const score &score : scores) {
            if (s.hash == score.hash || s.beatmap_md5 != score.beatmap_md5)
                continue;

            if (score.total_score > s.total_score)
                return true;
        }

        return false;
    }), scores.end());

    std::sort(scores.begin(), scores.end(), [](const score &s_left, const score &s_right) {
        return s_left.pp > s_right.pp;
    });

    if (scores.size() > 100)
        scores.resize(100);

    return scores;
}

std::optional<shiro::scores::score> shiro::scores::helper::get_latest_score(int32_t user_id, const utils::play_mode &mode) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(
            score_table.user_id == user_id and
            score_table.play_mode == (uint8_t) mode
    ).order_by(score_table.time.desc()).limit(1u));

    if (result.empty())
        return std::nullopt;

    // We are ordering by time (descending) so the front value will have the highest timestamp which is the latest in unix time
    const auto &row = result.front();

    score s(-1);

    s.id = row.id;
    s.user_id = row.user_id;
    s.hash = row.hash;
    s.beatmap_md5 = row.beatmap_md5;

    s.rank = row.ranking;
    s.total_score = row.score;
    s.max_combo = row.max_combo;
    s.pp = row.pp;

    s.accuracy = row.accuracy;
    s.mods = row.mods;

    s.fc = row.fc;
    s.passed = row.passed;

    s._300_count = row._300_count;
    s._100_count = row._100_count;
    s._50_count = row._50_count;
    s.katus_count = row.katus_count;
    s.gekis_count = row.gekis_count;
    s.miss_count = row.miss_count;

    s.play_mode = row.play_mode;
    s.time = row.time;

    return s;
}

shiro::scores::score shiro::scores::helper::get_score(int32_t id) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.id == id).limit(1u));

    if (result.empty())
        return score(-1);

    const auto &row = result.front();
    score s(-1);

    s.id = row.id;
    s.user_id = row.user_id;
    s.hash = row.hash;
    s.beatmap_md5 = row.beatmap_md5;

    s.rank = row.ranking;
    s.total_score = row.score;
    s.max_combo = row.max_combo;
    s.pp = row.pp;

    s.accuracy = row.accuracy;
    s.mods = row.mods;

    s.fc = row.fc;
    s.passed = row.passed;

    s._300_count = row._300_count;
    s._100_count = row._100_count;
    s._50_count = row._50_count;
    s.katus_count = row.katus_count;
    s.gekis_count = row.gekis_count;
    s.miss_count = row.miss_count;

    s.play_mode = row.play_mode;
    s.time = row.time;

    return s;
}

int32_t shiro::scores::helper::get_scoreboard_position(const shiro::scores::score &s, std::vector<score> scores) {
    std::sort(scores.begin(), scores.end(), [](const score &s_left, const score &s_right) {
        return s_left.total_score > s_right.total_score;
    });

    for (size_t i = 0; i < scores.size(); i++) {
        score &beatmap_score = scores.at(i);

        if (beatmap_score.id == s.id)
            return i + 1;
    }

    return -1;
}

bool shiro::scores::helper::is_ranked(const shiro::scores::score &score, const shiro::beatmaps::beatmap &beatmap) {
    bool ranked = true;
    int32_t mods = score.mods;

    switch ((utils::play_mode) score.play_mode) {
        case utils::play_mode::standard:
            ranked &= config::score_submission::std_ranked;
            break;
        case utils::play_mode::taiko:
            ranked &= config::score_submission::taiko_ranked;
            break;
        case utils::play_mode::fruits:
            ranked &= config::score_submission::catch_ranked;
            break;
        case utils::play_mode::mania:
            ranked &= config::score_submission::mania_ranked;
            break;
    }

    if (mods & (int32_t) utils::mods::none)
        ranked &= config::score_submission::nomod_ranked;

    if (mods & (int32_t) utils::mods::no_fail)
        ranked &= config::score_submission::no_fail_ranked;

    if (mods & (int32_t) utils::mods::easy)
        ranked &= config::score_submission::easy_ranked;

    if (mods & (int32_t) utils::mods::touch_device)
        ranked &= config::score_submission::touch_device_ranked;

    if (mods & (int32_t) utils::mods::hidden)
        ranked &= config::score_submission::hidden_ranked;

    if (mods & (int32_t) utils::mods::hard_rock)
        ranked &= config::score_submission::hard_rock_ranked;

    if (mods & (int32_t) utils::mods::sudden_death)
        ranked &= config::score_submission::sudden_death_ranked;

    if (mods & (int32_t) utils::mods::double_time)
        ranked &= config::score_submission::double_time_ranked;

    if (mods & (int32_t) utils::mods::relax)
        ranked &= config::score_submission::relax_ranked;

    if (mods & (int32_t) utils::mods::half_time)
        ranked &= config::score_submission::half_time_ranked;

    if (mods & (int32_t) utils::mods::nightcore)
        ranked &= config::score_submission::nightcore_ranked;

    if (mods & (int32_t) utils::mods::flashlight)
        ranked &= config::score_submission::flashlight_ranked;

    if (mods & (int32_t) utils::mods::auto_play)
        ranked &= config::score_submission::auto_play_ranked;

    if (mods & (int32_t) utils::mods::spun_out)
        ranked &= config::score_submission::spun_out_ranked;

    if (mods & (int32_t) utils::mods::auto_pilot)
        ranked &= config::score_submission::auto_pilot_ranked;

    if (mods & (int32_t) utils::mods::perfect)
        ranked &= config::score_submission::perfect_ranked;

    // Special mods

    if (mods & (int32_t) utils::mods::fade_in)
        ranked &= config::score_submission::fade_in_ranked;

    if (mods & (int32_t) utils::mods::random)
        ranked &= config::score_submission::random_ranked;

    if (mods & (int32_t) utils::mods::cinema)
        ranked &= config::score_submission::cinema_ranked;

    if (mods & (int32_t) utils::mods::target)
        ranked &= config::score_submission::target_ranked;

    if (mods & (int32_t) utils::mods::score_v2)
        ranked &= config::score_submission::score_v2_ranked;

    // Keys

    if (mods & (int32_t) utils::mods::key_1)
        ranked &= config::score_submission::key_1_ranked;

    if (mods & (int32_t) utils::mods::key_2)
        ranked &= config::score_submission::key_2_ranked;

    if (mods & (int32_t) utils::mods::key_3)
        ranked &= config::score_submission::key_3_ranked;

    if (mods & (int32_t) utils::mods::key_4)
        ranked &= config::score_submission::key_4_ranked;

    if (mods & (int32_t) utils::mods::key_5)
        ranked &= config::score_submission::key_5_ranked;

    if (mods & (int32_t) utils::mods::key_6)
        ranked &= config::score_submission::key_6_ranked;

    if (mods & (int32_t) utils::mods::key_7)
        ranked &= config::score_submission::key_7_ranked;

    if (mods & (int32_t) utils::mods::key_8)
        ranked &= config::score_submission::key_8_ranked;

    if (mods & (int32_t) utils::mods::key_9)
        ranked &= config::score_submission::key_9_ranked;

    if (mods & (int32_t) utils::mods::key_coop)
        ranked &= config::score_submission::key_coop_ranked;

    if (beatmap.id != 0)
        ranked &= beatmaps::helper::has_leaderboard(beatmaps::helper::fix_beatmap_status(beatmap.ranked_status));

    return ranked;
}

std::tuple<bool, std::string> shiro::scores::helper::is_flagged(const shiro::scores::score &score, const shiro::beatmaps::beatmap &beatmap) {
    if (!is_ranked(score, beatmap) || !score.passed)
        return { false, "" };

    if (config::score_submission::restrict_negative_values) {
        if (score.total_score < 0)
            return { true, "Negative score value (total score " + std::to_string(score.total_score) + " < 0)" };

        if (score.max_combo < 0)
            return { true, "Negative score value (max combo " + std::to_string(score.max_combo) + " < 0)" };

        if (score._300_count < 0)
            return { true, "Negative score value (300 count " + std::to_string(score._300_count) + " < 0)" };

        if (score._100_count < 0)
            return { true, "Negative score value (100 count " + std::to_string(score._100_count) + " < 0)" };

        if (score._50_count < 0)
            return { true, "Negative score value (50 count " + std::to_string(score._50_count) + " < 0)" };

        if (score.katus_count < 0)
            return { true, "Negative score value (katus count " + std::to_string(score.katus_count) + " < 0)" };

        if (score.gekis_count < 0)
            return { true, "Negative score value (gekis count " + std::to_string(score.gekis_count) + " < 0)" };

        if (score.miss_count < 0)
            return { true, "Negative score value (miss count " + std::to_string(score.miss_count) + " < 0)" };

        if (score.mods < 0)
            return { true, "Negative score value (mods " + std::to_string(score.mods) + " < 0)" };
    }

    if (config::score_submission::restrict_impossible_mods) {
        int32_t mods = score.mods;

        if (mods & (int32_t) utils::mods::easy && mods & (int32_t) utils::mods::hard_rock)
            return { true, "Impossible mod combination (EZ + HR)" };

        if (mods & (int32_t) utils::mods::no_fail && mods & (int32_t) utils::mods::sudden_death)
            return { true, "Impossible mod combination (NF + SD)" };

        if (mods & (int32_t) utils::mods::no_fail && mods & (int32_t) utils::mods::perfect)
            return { true, "Impossible mod combination (NF + PF)" };

        if (mods & (int32_t) utils::mods::half_time && mods & (int32_t) utils::mods::double_time)
            return { true, "Impossible mod combination (HT + DT)" };

        if (mods & (int32_t) utils::mods::sudden_death && mods & (int32_t) utils::mods::relax)
            return { true, "Impossible mod combination (SD + RX)" };

        if (mods & (int32_t) utils::mods::sudden_death && mods & (int32_t) utils::mods::auto_pilot)
            return { true, "Impossible mod combination (SD + AP)" };

        if (mods & (int32_t) utils::mods::sudden_death && mods & (int32_t) utils::mods::auto_play)
            return { true, "Impossible mod combination (SD + AUTO)" };

        if (mods & (int32_t) utils::mods::relax && mods & (int32_t) utils::mods::no_fail)
            return { true, "Impossible mod combination (RX + NF)" };

        if (mods & (int32_t) utils::mods::relax && mods & (int32_t) utils::mods::auto_pilot)
            return { true, "Impossible mod combination (RX + AP)" };

        if (mods & (int32_t) utils::mods::relax && mods & (int32_t) utils::mods::auto_play)
            return { true, "Impossible mod combination (RX + AUTO)" };

        if (mods & (int32_t) utils::mods::auto_pilot && mods & (int32_t) utils::mods::no_fail)
            return { true, "Impossible mod combination (AP + NF)" };

        if (mods & (int32_t) utils::mods::auto_pilot && mods & (int32_t) utils::mods::spun_out)
            return { true, "Impossible mod combination (AP + SO)" };

        if (mods & (int32_t) utils::mods::auto_pilot && mods & (int32_t) utils::mods::auto_play)
            return { true, "Impossible mod combination (AP + AUTO)" };

        if (mods & (int32_t) utils::mods::spun_out && mods & (int32_t) utils::mods::auto_play)
            return { true, "Impossible mod combination (SO + AUTO)" };
    }

    bool impossible_combo = score.play_mode == (uint8_t) utils::play_mode::standard &&
            beatmap.max_combo > 0 &&
            score.max_combo > beatmap.max_combo;

    if (config::score_submission::restrict_impossible_combo && impossible_combo) {
        std::string reason = "Impossible combo (" + std::to_string(score.max_combo) + " > " + std::to_string(beatmap.max_combo) + ")";
        return { true, reason };
    }

    switch ((utils::play_mode) score.play_mode) {
        case utils::play_mode::standard:
            if (config::score_submission::auto_restrict_pp_std > -1 && score.pp > config::score_submission::auto_restrict_pp_std) {
                std::string reason = "High pp gain (osu!std, " + std::to_string(score.pp) + "pp > " + std::to_string(config::score_submission::auto_restrict_pp_std) + "pp)";
                return { true, reason };
            }
            break;
        case utils::play_mode::taiko:
            if (config::score_submission::auto_restrict_pp_taiko > -1 && score.pp > config::score_submission::auto_restrict_pp_taiko) {
                std::string reason = "High pp gain (osu!taiko, " + std::to_string(score.pp) + "pp > " + std::to_string(config::score_submission::auto_restrict_pp_taiko) + "pp)";
                return { true, reason };
            }
            break;
        case utils::play_mode::fruits:
            if (config::score_submission::auto_restrict_pp_catch > -1 && score.pp > config::score_submission::auto_restrict_pp_catch) {
                std::string reason = "High pp gain (osu!catch, " + std::to_string(score.pp) + "pp > " + std::to_string(config::score_submission::auto_restrict_pp_catch) + "pp)";
                return { true, reason };
            }
            break;
        case utils::play_mode::mania:
            if (config::score_submission::auto_restrict_pp_mania > -1 && score.pp > config::score_submission::auto_restrict_pp_mania) {
                std::string reason = "High pp gain (osu!mania, " + std::to_string(score.pp) + "pp > " + std::to_string(config::score_submission::auto_restrict_pp_mania) + "pp)";
                return { true, reason };
            }
            break;
    }

    return { false, "" };
}

float shiro::scores::helper::calculate_accuracy(utils::play_mode mode, int32_t _300, int32_t _100, int32_t _50, int32_t geki, int32_t katu, int32_t miss) {
    // TODO: Use performance library for accuracy calculation
    // https://github.com/Marc3842h/hikari/blob/master/src/accuracy.rs
    // https://github.com/Marc3842h/hikari/blob/982260f83309499e2239148800561548f3fa39cf/include/hikari.h#L49

    switch (mode) {
        case utils::play_mode::standard: {
            int32_t total_hits = _300 + _100 + _50 + miss;

            if (total_hits <= 0)
                return 0.0f;

            float accuracy = (_300 * 300.0f + _100 * 100.0f + _50 * 50.0f) / (total_hits * 300.0f);
            return accuracy * 100;
        }
        case utils::play_mode::taiko: {
            int32_t _150 = _100 + _50;
            int32_t total_hits = _300 + _150 + miss;

            if (total_hits <= 0)
                return 0.0f;

            float accuracy = (_300 * 300.0f + _150 * 150.0f) / (total_hits * 300.0f);
            return accuracy * 100;
        }
        case utils::play_mode::fruits: {
            int32_t total = _50 + _100 + _300 + katu + miss;

            if (total <= 0)
                return 0.0;

            int32_t numerator = _50 + _100 + _300;
            float denominator = total;

            return (numerator / denominator) * 100;
        }
        case utils::play_mode::mania: {
            int32_t total = _50 + _100 + katu + _300 + geki + miss;

            if (total <= 0)
                return 0.0;

            int32_t numerator = 50 * _50 + 100 * _100 + 200 * katu + 300 * (_300 + geki);
            float denominator = 300 * total;

            return (numerator / denominator) * 100;
        }
    }

    return 0.0f;
}
