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

#ifndef SHIRO_SCORE_HELPER_HH
#define SHIRO_SCORE_HELPER_HH

#include <memory>
#include <string>
#include <vector>

#include "../beatmaps/beatmap.hh"
#include "../users/user.hh"
#include "score.hh"

namespace shiro::scores::helper {

    score fetch_top_score_user(std::string beatmap_md5sum, std::shared_ptr<users::user> user);

    std::vector<score> fetch_all_scores(std::string beatmap_md5sum, size_t limit = 50);

    std::vector<score> fetch_country_scores(std::string beatmap_md5sum, uint8_t country, size_t limit = 50);

    std::vector<score> fetch_mod_scores(std::string beatmap_md5sum, int32_t mods, size_t limit = 50);

    std::vector<score> fetch_friend_scores(std::string beatmap_md5sum, std::shared_ptr<users::user> user, size_t limit = 50);

    std::vector<score> fetch_user_scores(std::string beatmap_md5sum, std::shared_ptr<users::user> user, size_t limit = std::numeric_limits<size_t>::max());

    int32_t get_scoreboard_position(const score &s, std::vector<score> scores);

    std::vector<score> fetch_all_user_scores(int32_t user_id, size_t limit = std::numeric_limits<size_t>::max());

    // This method sorts using the pp value instead of score (like all other methods above)
    std::vector<score> fetch_top100_user(utils::play_mode mode, int32_t user_id);

    score get_score(int32_t id);

    bool is_ranked(const score &score, const beatmaps::beatmap &beatmap);

    std::tuple<bool, std::string> is_flagged(const score &score, const beatmaps::beatmap &beatmap);

    float calculate_accuracy(utils::play_mode mode, int32_t _300, int32_t _100, int32_t _50, int32_t geki, int32_t katu, int32_t miss);

}

#endif //SHIRO_SCORE_HELPER_HH
