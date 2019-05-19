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

#ifndef SHIRO_TABLE_DISPLAY_HH
#define SHIRO_TABLE_DISPLAY_HH

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>

#include "../beatmaps/beatmap.hh"
#include "../users/user.hh"

namespace shiro::scores {

    class table_display {
    private:
        std::shared_ptr<users::user> user = nullptr;
        beatmaps::beatmap beatmap;
        scores::score score;
        bool legacy = false;

        std::ostringstream output;

        int32_t old_scoreboard_pos = 0;
        int32_t old_max_combo = 0;
        float old_acc = 0.0f;
        int32_t old_rank = 0;
        int16_t old_pp = 0;
        scores::score old_top_score;

        int32_t scoreboard_position = 0;

    public:
        table_display(std::shared_ptr<users::user> user, beatmaps::beatmap map, scores::score score, bool legacy);

        void init();

        void set_scoreboard_position(int32_t position);

        template <typename T>
        void push(const std::string &key, T value, bool flush = false) {
            this->output << key << ":" << value;

            if (!flush) {
                this->output << "|";
                return;
            }

            this->output << std::endl;
        }

        std::string build_present();
        std::string build_legacy();
        std::string build();

    };

}

#endif //SHIRO_TABLE_DISPLAY_HH
