#define OPPAI_IMPLEMENTATION

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "../../../beatmaps/beatmap.hh"
#include "../../../beatmaps/beatmap_helper.hh"
#include "../../../database/tables/score_table.hh"
#include "../../../ranking/ranking_helper.hh"
#include "../../../replays/replay_manager.hh"
#include "../../../scores/score.hh"
#include "../../../scores/score_helper.hh"
#include "../../../thirdparty/loguru.hh"
#include "../../../thirdparty/oppai.hh"
#include "../../../users/user.hh"
#include "../../../users/user_manager.hh"
#include "../../../utils/bot_utils.hh"
#include "../../../utils/crypto.hh"
#include "../../../utils/multipart_parser.hh"
#include "../../../utils/string_utils.hh"
#include "submit_score_route.hh"

void shiro::routes::web::submit_score::handle(const crow::request &request, crow::response &response) {
    response.set_header("Content-Type", "text/plain; charset=UTF-8");
    response.set_header("cho-server", "shiro (https://github.com/Marc3842h/shiro)");

    const std::string &user_agent = request.get_header_value("user-agent");

    if (user_agent.empty() || user_agent != "osu!") {
        response.code = 403;
        response.end();

        LOG_F(WARNING, "Received score submission from %s without osu! user agent.", request.get_header_value("X-Forwarded-For").c_str());
        return;
    }

    const std::string &content_type = request.get_header_value("content-type");

    if (content_type.empty()) {
        response.code = 400;
        response.end("error: invalid");

        LOG_S(ERROR) << "Received score submission without content type.";
        return;
    }

    std::unique_ptr<utils::multipart_parser> parser = std::make_unique<utils::multipart_parser>(request.body, content_type);

    if (parser == nullptr)
        return;

    utils::multipart_fields fields = parser->parse();
    std::string key = "h89f2-890h2h89b34g-h80g134n90133";

    if (fields.find("pass") == fields.end()) {
        response.code = 403;
        response.end("error: pass");

        LOG_S(WARNING) << "Received score submission without password.";
        return;
    }

    if (fields.find("score") == fields.end()) {
        response.code = 400;
        response.end("error: invalid");

        LOG_S(WARNING) << "Received score without score data.";
        return;
    }

    if (fields.find("osuver") != fields.end())
        key = "osu!-scoreburgr---------" + fields.at("osuver").content;

    std::vector<unsigned char> decrypted = utils::crypto::rijndael256::decode(
            utils::crypto::base64::decode(fields.at("iv").content.c_str()),
            key,
            utils::crypto::base64::decode(fields.at("score").content.c_str())
    );
    std::string result(reinterpret_cast<char*>(&decrypted[0]), decrypted.size());

    std::vector<std::string> score_metadata;
    boost::split(score_metadata, result, boost::is_any_of(":"));

    if (score_metadata.size() < 16) {
        response.code = 400;
        response.end("error: invalid");

        LOG_S(WARNING) << "Received invalid score submission, score metadata doesn't have 16 or more parts.";
        return;
    }

    boost::trim_right(score_metadata.at(1));

    std::shared_ptr<users::user> user = users::manager::get_user_by_username(score_metadata.at(1));

    // This only occurs when the server restarted and osu submitted before being re-logged in
    if (user == nullptr) {
        response.code = 403;

        LOG_S(WARNING) << "Received score submission from offline user.";
        return;
    }

    if (!user->check_password(fields.at("pass").content)) {
        response.code = 403;
        response.end("error: pass");

        LOG_F(WARNING, "Received score submission from %s with incorrect password.", user->presence.username.c_str());
        return;
    }

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::scores score_table {};

    int32_t game_version = 20131216;

    score_metadata.at(17).erase(std::remove_if(score_metadata.at(17).begin(), score_metadata.at(17).end(), [](char c) {
        return !std::isdigit(c);
    }), score_metadata.at(17).end());

    scores::score score;
    score.user_id = user->user_id;

    score.beatmap_md5 = score_metadata.at(0);
    score.hash = score_metadata.at(2);

    try {
        score._300_count = boost::lexical_cast<int32_t>(score_metadata.at(3));
        score._100_count = boost::lexical_cast<int32_t>(score_metadata.at(4));
        score._50_count = boost::lexical_cast<int32_t>(score_metadata.at(5));
        score.gekis_count = boost::lexical_cast<int32_t>(score_metadata.at(6));
        score.katus_count = boost::lexical_cast<int32_t>(score_metadata.at(7));
        score.miss_count = boost::lexical_cast<int32_t>(score_metadata.at(8));
        score.total_score = boost::lexical_cast<int64_t>(score_metadata.at(9));
        score.max_combo = boost::lexical_cast<int32_t>(score_metadata.at(10));
        score.mods = boost::lexical_cast<int32_t>(score_metadata.at(13));
        score.play_mode = static_cast<uint8_t>(boost::lexical_cast<int32_t>(score_metadata.at(15)));
    } catch (const boost::bad_lexical_cast &ex) {
        response.code = 500;
        response.end();

        LOG_F(WARNING, "Received score submission from %s with invalid types.", user->presence.username.c_str());
        return;
    }

    try {
        game_version = boost::lexical_cast<int32_t>(score_metadata.at(17));
    } catch (const boost::bad_lexical_cast &ex) {
        LOG_S(WARNING) << "Unable to convert " << score_metadata.at(17) << " to game version: " << ex.what();
    }

    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    score.rank = score_metadata.at(12);
    score.fc = utils::strings::to_bool(score_metadata.at(11));
    score.passed = utils::strings::to_bool(score_metadata.at(14));
    score.time = seconds.count();

    score.accuracy = scores::helper::calculate_accuracy(
            (utils::play_mode) score.play_mode,
            score._300_count, score._100_count, score._50_count,
            score.gekis_count, score.katus_count, score.miss_count
    );

    auto db_result = db(select(all_of(score_table)).from(score_table).where(score_table.hash == score.hash));
    bool empty = is_query_empty(db_result);

    // Score has already been submitted
    if (!empty) {
        response.end("error: dup");

        LOG_F(WARNING, "%s resubmitted a previously submitted score.", user->presence.username.c_str());
        return;
    }

    user->stats.play_count++;

    beatmaps::beatmap beatmap;
    beatmap.beatmap_md5 = score.beatmap_md5;

    beatmap.fetch();

    if (score.passed)
        beatmap.pass_count++;

    beatmap.play_count++;
    beatmap.update_play_metadata();

    if (!score.passed) {
        user->refresh_stats();
        response.end("ok");
        return;
    }

    if (fields.find("replay") == fields.end()) {
        response.code = 400;
        response.end("error: invalid");

        LOG_S(WARNING) << "Received score without replay data.";
        return;
    }

    // oppai-ng for std and taiko (non-converted)
    if ((score.play_mode == (uint8_t) utils::play_mode::standard || score.play_mode == (uint8_t) utils::play_mode::taiko) &&
        beatmaps::helper::awards_pp(beatmap.ranked_status)) {
        struct parser parser_state;
        struct beatmap map;

        struct diff_calc difficulty;
        struct pp_calc pp;

        struct pp_params params;

        p_init(&parser_state);
        p_map(&parser_state, &map, beatmaps::helper::download(beatmap.beatmap_id));

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
        params.score_version = PP_DEFAULT_SCORING;

        ppv2p(&pp, &params);

        score.pp = pp.total;

        d_free(&difficulty);
        p_free(&parser_state);
    } else {
        score.pp = 0;
    }

    scores::score old_top_score = scores::helper::fetch_top_score_user(beatmap.beatmap_md5, user);
    int32_t old_scoreboard_pos = scores::helper::get_scoreboard_position(old_top_score, scores::helper::fetch_all_scores(beatmap.beatmap_md5, 5));

    if (old_scoreboard_pos == -1)
        old_scoreboard_pos = 0;

    db(insert_into(score_table).set(
            score_table.user_id = score.user_id,
            score_table.hash = score.hash,
            score_table.beatmap_md5 = score.beatmap_md5,
            score_table.rank = score.rank,
            score_table.score = score.total_score,
            score_table.max_combo = score.max_combo,
            score_table.pp = score.pp,
            score_table.accuracy = score.accuracy,
            score_table.mods = score.mods,
            score_table.fc = score.fc,
            score_table.passed = score.passed,
            score_table._300_count = score._300_count,
            score_table._100_count = score._100_count,
            score_table._50_count = score._50_count,
            score_table.katus_count = score.katus_count,
            score_table.gekis_count = score.gekis_count,
            score_table.miss_count = score.miss_count,
            score_table.play_mode = score.play_mode,
            score_table.time = score.time
    ));

    db_result = db(select(all_of(score_table)).from(score_table).where(score_table.hash == score.hash));
    empty = is_query_empty(db_result);

    if (empty) {
        response.end("error: invalid");
        return;
    }

    // Just to get the id
    for (const auto &row : db_result) {
        score.id = row.id;
    }

    user->stats.total_score += score.total_score;

    if (!beatmaps::helper::has_leaderboard(beatmaps::helper::fix_beatmap_status(beatmap.ranked_status))) {
        response.end("ok" /*"error: disabled"*/);
        return;
    }

    scores::score top_score = scores::helper::fetch_top_score_user(beatmap.beatmap_md5, user);
    int32_t scoreboard_position = scores::helper::get_scoreboard_position(top_score, scores::helper::fetch_all_scores(beatmap.beatmap_md5, 5));

    if (top_score.hash == score.hash) {
        if (scoreboard_position == 1) {
            char buffer[1024];
            std::snprintf(
                    buffer, sizeof(buffer),
                    "[https://shiro.host/u/%i %s] achieved rank #1 on [https://osu.ppy.sh/b/%i %s] (%s)",
                    user->user_id, user->presence.username.c_str(),
                    beatmap.beatmap_id, beatmap.song_name.c_str(),
                    utils::play_mode_to_string((utils::play_mode) score.play_mode).c_str()
            );

            utils::bot::respond(buffer, user, "#announce", false);
        }
    }

    user->stats.ranked_score += score.total_score;

    user->stats.recalculate_pp();

    float old_acc = user->stats.accuracy;
    user->stats.recalculate_accuracy();

    user->save_stats();

    int32_t old_rank = user->stats.rank;
    user->stats.rank = ranking::helper::get_leaderboard_position(user->stats.play_mode, user->presence.username);
    user->presence.rank = user->stats.rank;

    std::string user_above = ranking::helper::get_leaderboard_user(user->stats.play_mode, user->stats.rank - 1);
    int16_t user_above_pp = ranking::helper::get_pp_for_user(user->stats.play_mode, user_above);

    // Save replay
    replays::save_replay(score, game_version, fields.at("replay").content);

    uint32_t timestamp = static_cast<uint32_t>(beatmap.last_update);
    std::time_t time = timestamp;

    struct std::tm *tm = std::gmtime(&time);
    std::stringstream out;

    out << "beatmapId:" << beatmap.beatmap_id << "|";
    out << "beatmapSetId:" << beatmap.beatmapset_id << "|";
    out << "beatmapPlaycount:" << beatmap.play_count << "|";
    out << "beatmapPasscount:" << beatmap.pass_count << "|";
    out << "approvedDate:" << std::put_time(tm, "%F %X") << "|";
    out << "chartId:overall" << "|";
    out << "chartName:Overall Ranking" << "|";
    out << "chartEndDate:" << "|";
    out << "beatmapRankingBefore:" << old_scoreboard_pos << "|";
    out << "beatmapRankingAfter:" << scoreboard_position << "|";
    out << "rankedScoreBefore:" << user->stats.ranked_score - score.total_score << "|";
    out << "rankedScoreAfter:" << user->stats.ranked_score << "|";
    out << "totalScoreBefore:" << user->stats.total_score - score.total_score << "|";
    out << "totalScoreAfter:" << user->stats.total_score << "|";
    out << "playCountBefore:" << user->stats.play_count << "|";
    out << "accuracyBefore:" << old_acc << "|";
    out << "accuracyAfter:" << user->stats.accuracy << "|";
    out << "rankBefore:" << old_rank << "|";
    out << "rankAfter:" << user->stats.rank << "|";
    out << "toNextRank:" << user_above_pp - user->stats.pp << "|";
    out << "toNextRankUser:" << user_above << "|";
    out << "achievements:0" << "|";
    out << "achievements-new:" << "|";
    out << "onlineScoreId:" << score.id << "|";

    user->refresh_stats();
    response.end(out.str());
}
