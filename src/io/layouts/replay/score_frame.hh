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

#ifndef SHIRO_SCORE_FRAME_HH
#define SHIRO_SCORE_FRAME_HH

#include "../serializable.hh"

namespace shiro::io::layouts {

    class score_frame : public serializable {
    public:
        int32_t time = 0;
        uint8_t id = 0;

        uint16_t count_300 = 0;
        uint16_t count_100 = 0;
        uint16_t count_50 = 0;
        uint16_t count_geki = 0;
        uint16_t count_katu = 0;
        uint16_t count_miss = 0;

        int32_t total_score = 0;
        uint16_t max_combo = 0;
        uint16_t current_combo = 0;
        bool perfect = false;

        uint8_t current_hp = 0;
        uint8_t tag_byte = 0;
        bool score_v2 = false;

        double combo_portion = 0.0;
        double bonus_portion = 0.0;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;
    };

}

#endif //SHIRO_SCORE_FRAME_HH
