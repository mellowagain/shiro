#include "score_helper.hh"
#include "../database/tables/score_table.hh"
#include "../users/user_manager.hh"
#include "../geoloc/country_ids.hh"
#include "../thirdparty/loguru.hh"
#include "../thirdparty/oppai.hh"

shiro::scores::score shiro::scores::helper::fetch_top_score_user(std::string beatmap_md5sum, std::shared_ptr<shiro::users::user> user) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.beatmap_md5 == beatmap_md5sum and score_table.user_id == user->user_id));
    bool empty = is_query_empty(result);

    if (empty)
        return score(-1);

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = beatmap_md5sum;

        s.rank = row.rank;
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

    return scores.at(0);
}

std::vector<shiro::scores::score> shiro::scores::helper::fetch_all_scores(std::string beatmap_md5sum, size_t limit) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.beatmap_md5 == beatmap_md5sum));
    bool empty = is_query_empty(result);

    if (empty)
        return {};

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = beatmap_md5sum;

        s.rank = row.rank;
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
    bool empty = is_query_empty(result);

    if (empty)
        return {};

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = beatmap_md5sum;

        s.rank = row.rank;
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

    scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const score &s) {
        std::shared_ptr<users::user> user = users::manager::get_user_by_id(s.user_id);

        if (user == nullptr) {
            user = std::make_shared<users::user>(s.user_id);

            if (!user->init())
                return true;
        }

        uint8_t user_country = geoloc::get_country_id(user->country);
        return user_country != country;
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
    bool empty = is_query_empty(result);

    if (empty)
        return {};

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = beatmap_md5sum;

        s.rank = row.rank;
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

    scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const score &s) {
        return s.mods != mods;
    }), scores.end());

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

std::vector<shiro::scores::score> shiro::scores::helper::fetch_friend_scores(std::string beatmap_md5sum, std::shared_ptr<shiro::users::user> user, size_t limit) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.beatmap_md5 == beatmap_md5sum));
    bool empty = is_query_empty(result);

    if (empty)
        return {};

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = beatmap_md5sum;

        s.rank = row.rank;
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

std::vector<shiro::scores::score> shiro::scores::helper::fetch_all_user_scores(int32_t user_id, size_t limit) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.user_id == user_id));
    bool empty = is_query_empty(result);

    if (empty)
        return {};

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = row.beatmap_md5;

        s.rank = row.rank;
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

    if (scores.size() > limit)
        scores.resize(limit);

    return scores;
}

std::vector<shiro::scores::score> shiro::scores::helper::fetch_top100_user(shiro::utils::play_mode mode, int32_t user_id) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    auto result = db(select(all_of(score_table)).from(score_table).where(score_table.user_id == user_id and score_table.play_mode == (uint8_t) mode));
    bool empty = is_query_empty(result);

    if (empty)
        return {};

    std::vector<score> scores;

    for (const auto &row : result) {
        score s;

        s.id = row.id;
        s.user_id = row.user_id;
        s.hash = row.hash;
        s.beatmap_md5 = row.beatmap_md5;

        s.rank = row.rank;
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
        return s_left.pp > s_right.pp;
    });

    if (scores.size() > 100)
        scores.resize(100);

    return scores;
}

float shiro::scores::helper::calculate_accuracy(utils::play_mode mode, int32_t _300, int32_t _100, int32_t _50, int32_t geki, int32_t katu, int32_t miss) {
    switch (mode) {
        case utils::play_mode::standard: {
            return acc_calc(_300, _100, _50, miss) * 100;
        }
        case utils::play_mode::taiko: {
            return taiko_acc_calc(_300, _100 + _50, miss) * 100;
        }
        case utils::play_mode::fruits: {
            int32_t total_points = (_300 + _100 + _50);
            int32_t total_hits = total_points + miss + katu;

            return (total_points / total_hits) * 100;
        }
        case utils::play_mode::mania: {
            int32_t total_points = (_300 * 300) + (geki * 300) + (_100 * 100) + (katu * 200) + (_50 * 50);
            int32_t total_hits = _300 + geki + _100 + katu + _50 + miss;

            return (total_points / (total_hits * 300)) * 100;
        }
    }

    return 0.0f;
}
