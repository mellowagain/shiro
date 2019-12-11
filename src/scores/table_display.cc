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

#include <iomanip>
#include <sstream>
#include <utility>

#include "../ranking/ranking_helper.hh"
#include "score_helper.hh"
#include "table_display.hh"

shiro::scores::table_display::table_display(std::shared_ptr<users::user> user, beatmaps::beatmap beatmap, scores::score score, bool legacy)
    : user(std::move(user))
    , beatmap(std::move(beatmap))
    , score(std::move(score))
    , legacy(legacy) {
    this->output << std::setprecision(12);
}

void shiro::scores::table_display::init() {
    scores::score old_top_score = helper::fetch_top_score_user(this->beatmap.beatmap_md5, this->user);
    this->old_top_score = old_top_score;
    this->old_scoreboard_pos = helper::get_scoreboard_position(old_top_score, helper::fetch_all_scores(this->beatmap.beatmap_md5, 5));

    if (this->old_scoreboard_pos == -1)
        this->old_scoreboard_pos = 0;

    this->old_max_combo = this->user->stats.max_combo;
    this->old_acc = this->user->stats.accuracy;
    this->old_rank = this->user->stats.rank;
    this->old_pp = this->user->stats.pp;
}

void shiro::scores::table_display::set_scoreboard_position(int32_t position) {
    this->scoreboard_position = position;
}

std::string shiro::scores::table_display::build_present() {
    std::time_t time = (uint32_t) this->beatmap.last_update;
    struct std::tm *tm = std::gmtime(&time);

    push("beatmapId", this->beatmap.beatmap_id);
    push("beatmapSetId", this->beatmap.beatmapset_id);
    push("beatmapPlaycount", this->beatmap.play_count);
    push("beatmapPasscount", this->beatmap.pass_count);
    push("approvedDate", std::put_time(tm, "%F %X"), true);

    push("chartId", "beatmap");
    push("chartUrl", this->beatmap.get_url());
    push("chartName", "Beatmap Ranking");
    push("rankBefore", this->old_scoreboard_pos);
    push("rankAfter", this->scoreboard_position);
    push("maxComboBefore", this->old_top_score.max_combo);
    push("maxComboAfter", this->score.max_combo);
    push("accuracyBefore", this->old_top_score.accuracy);
    push("accuracyAfter", this->score.accuracy);
    push("rankedScoreBefore", this->old_top_score.total_score); // Gets only sent if the map is ranked
    push("rankedScoreAfter", this->score.total_score); // Gets only sent if the map is ranked
    push("totalScoreBefore", this->old_top_score.total_score);
    push("totalScoreAfter", this->score.total_score);
    push("ppBefore", this->old_top_score.pp);
    push("ppAfter", this->score.pp);
    push("onlineScoreId", this->score.id, true);

    push("chartId", "overall");
    push("chartUrl", this->user->get_url());
    push("chartName", "Overall Ranking");
    push("rankBefore", this->old_rank);
    push("rankAfter", this->user->stats.rank);
    push("rankedScoreBefore", this->user->stats.ranked_score - this->score.total_score);
    push("rankedScoreAfter", this->user->stats.ranked_score);
    push("totalScoreBefore", this->user->stats.total_score - this->score.total_score);
    push("totalScoreAfter", this->user->stats.total_score);
    push("maxComboBefore", this->old_max_combo);
    push("maxComboAfter", this->user->stats.max_combo);
    push("accuracyBefore", this->old_acc);
    push("accuracyAfter", this->user->stats.accuracy);
    push("ppBefore", this->old_pp);
    push("ppAfter", this->user->stats.pp);
    push("achievements-new", "");
    push("onlineScoreId", this->score.id, true);

    return this->output.str();
}

std::string shiro::scores::table_display::build_legacy() {
    std::time_t time = (uint32_t) this->beatmap.last_update;
    struct std::tm *tm = std::gmtime(&time);

    push("beatmapId", this->beatmap.beatmap_id);
    push("beatmapSetId", this->beatmap.beatmapset_id);
    push("beatmapPlaycount", this->beatmap.play_count);
    push("beatmapPasscount", this->beatmap.pass_count);
    push("approvedDate", std::put_time(tm, "%F %X"), true);

    std::string user_above;
    int16_t user_above_pp = 0;

    #if defined(SEPARATE_RX_LEADERBOARDS)
        if (this->user->is_relax()) {
            user_above = ranking::helper::get_leaderboard_user_rx(this->user->stats.play_mode, this->user->stats.rank - 1);
            user_above_pp = ranking::helper::get_pp_for_user_rx(this->user->stats.play_mode, user_above);
        }
    #endif

    #if defined(SEPARATE_AP_LEADERBOARDS)
        if (this->user->is_auto_pilot()) {
            user_above = ranking::helper::get_leaderboard_user_ap(this->user->stats.play_mode, this->user->stats.rank - 1);
            user_above_pp = ranking::helper::get_pp_for_user_ap(this->user->stats.play_mode, user_above);
        }
    #endif

    // Integer comparision is usually faster than String comparision
    if (user_above_pp != 0) {
        user_above = ranking::helper::get_leaderboard_user(this->user->stats.play_mode, this->user->stats.rank - 1);
        user_above_pp = ranking::helper::get_pp_for_user(this->user->stats.play_mode, user_above);
    }

    int32_t to_next_rank = user_above_pp - this->user->stats.pp;

    if (to_next_rank < 0)
        to_next_rank = 0;

    push("chartId", "overall");
    push("chartName", "Overall Ranking");
    push("chartEndDate", "");
    push("beatmapRankingBefore", this->old_scoreboard_pos);
    push("beatmapRankingAfter", this->scoreboard_position);
    push("rankedScoreBefore", this->user->stats.ranked_score - this->score.total_score);
    push("rankedScoreAfter", this->user->stats.ranked_score);
    push("totalScoreBefore", this->user->stats.total_score - this->score.total_score);
    push("totalScoreAfter", this->user->stats.total_score);
    push("playCountBefore", this->user->stats.play_count);
    push("accuracyBefore", this->old_acc / 100);
    push("accuracyAfter", this->user->stats.accuracy / 100);
    push("rankBefore", this->old_rank);
    push("rankAfter", this->user->stats.rank);
    push("toNextRank", to_next_rank);
    push("toNextRankUser", user_above);
    push("achievements", "");
    push("achievements-new", "");
    push("onlineScoreId", this->score.id, true);

    return this->output.str();
}

std::string shiro::scores::table_display::build() {
    return this->legacy ? build_legacy() : build_present();
}
