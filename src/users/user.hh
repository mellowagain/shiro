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

#ifndef SHIRO_USER_HH
#define SHIRO_USER_HH

#include <chrono>

#include "../config/bot_file.hh"
#include "../io/layouts/user/user_presence.hh"
#include "../io/layouts/user/user_stats.hh"
#include "../io/layouts/user/user_status.hh"
#include "../io/queue.hh"
#include "../utils/play_mode.hh"
#include "../shiro.hh"

namespace shiro::users {

    class user {
    public:
        int32_t user_id = 0;
        std::string password = "";
        std::string salt = "";
        std::string country = "XX";

        uint32_t roles = 0;
        std::vector<int32_t> friends {};

        // Session info
        std::string token = ""; // UUID v4
        std::string hwid = ""; // SHA256
        std::chrono::seconds last_ping = 0s;
        bool hidden = false; // If the client is hidden, e.g. restricted

        std::string client_version = ""; // Full client version string sent on login (b20160403.6)
        int32_t client_build = 20131216; // Fixed client build version (20160403)
        uint32_t client_type = 0;

        io::layouts::user_presence presence;
        io::layouts::user_stats stats;
        io::layouts::user_status status;

        io::queue queue;

        explicit user(int32_t user_id);
        explicit user(const std::string &username);

        bool init();
        void update();
        void save_stats();

        #if defined(SEPARATE_RX_LEADERBOARDS)
            bool is_relax();
        #endif

        #if defined(SEPARATE_AP_LEADERBOARDS)
            bool is_auto_pilot();
        #endif

        std::string get_url();

        bool check_password(const std::string &password);

        // Re-sends presence and stats to the user to refresh on client-side
        void refresh_stats();

        template <typename t>
        inline void fill_local_stats(t remote_stats) {
            if (this->status.play_mode == (uint8_t) utils::play_mode::standard) {
                this->stats.pp = remote_stats.pp_std;
                this->stats.total_score = remote_stats.score_std;
                this->stats.ranked_score = remote_stats.ranked_score_std;
                this->stats.play_count = remote_stats.play_count_std;
                this->stats.rank = remote_stats.rank_std;
                this->presence.rank = remote_stats.rank_std;
                this->stats.max_combo = remote_stats.max_combo_std;
                this->stats.accuracy = remote_stats.accuracy_std;
            } else if (this->status.play_mode == (uint8_t) utils::play_mode::taiko) {
                this->stats.pp = remote_stats.pp_taiko;
                this->stats.total_score = remote_stats.score_taiko;
                this->stats.ranked_score = remote_stats.ranked_score_taiko;
                this->stats.play_count = remote_stats.play_count_taiko;
                this->stats.rank = remote_stats.rank_taiko;
                this->presence.rank = remote_stats.rank_taiko;
                this->stats.max_combo = remote_stats.max_combo_taiko;
                this->stats.accuracy = remote_stats.accuracy_taiko;
            } else if (this->status.play_mode == (uint8_t) utils::play_mode::fruits) {
                this->stats.pp = remote_stats.pp_ctb;
                this->stats.total_score = remote_stats.score_ctb;
                this->stats.ranked_score = remote_stats.ranked_score_ctb;
                this->stats.play_count = remote_stats.play_count_ctb;
                this->stats.rank = remote_stats.rank_ctb;
                this->presence.rank = remote_stats.rank_ctb;
                this->stats.max_combo = remote_stats.max_combo_ctb;
                this->stats.accuracy = remote_stats.accuracy_ctb;
            } else if (this->status.play_mode == (uint8_t) utils::play_mode::mania) {
                this->stats.pp = remote_stats.pp_mania;
                this->stats.total_score = remote_stats.score_mania;
                this->stats.ranked_score = remote_stats.ranked_score_mania;
                this->stats.play_count = remote_stats.play_count_mania;
                this->stats.rank = remote_stats.rank_mania;
                this->presence.rank = remote_stats.rank_mania;
                this->stats.max_combo = remote_stats.max_combo_mania;
                this->stats.accuracy = remote_stats.accuracy_mania;
            }
        }

    };

}

#endif //SHIRO_USER_HH
