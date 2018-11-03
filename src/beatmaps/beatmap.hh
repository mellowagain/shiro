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

#ifndef SHIRO_BEATMAP_HH
#define SHIRO_BEATMAP_HH

#include <cstdint>
#include <string>

#include "../scores/score.hh"
#include "../utils/play_mode.hh"

namespace shiro::beatmaps {

    class beatmap {
    public:
        int32_t id = 0;
        int32_t beatmap_id = 0;
        int32_t beatmapset_id = 0;
        uint8_t play_mode = (uint8_t) utils::play_mode::standard;

        std::string beatmap_md5 = "";
        std::string song_name = "";

        float ar = 0.0f;
        float od = 0.0f;

        float diff_std = 0.0f;
        float diff_taiko = 0.0f;
        float diff_ctb = 0.0f;
        float diff_mania = 0.0f;

        int32_t max_combo = 0;
        int32_t hit_length = 0;
        int32_t bpm = 0;

        int32_t ranked_status = -2;
        bool ranked_status_freezed = false;

        int32_t last_update = 0;

        int32_t play_count = 0;
        int32_t pass_count = 0;

        beatmap() = default;

        void fetch(bool force_peppster = false);
        bool fetch_db();
        bool fetch_api();

        // Saves beatmap meta data into the database
        void save();

        void update_play_metadata();

        // Builds beatmap header with default pass count of beatmap
        std::string build_header();

        // Builds beatmap with score count instead of default pass count
        std::string build_header(const std::vector<scores::score> &scores);

    };

}

#endif //SHIRO_BEATMAP_HH
