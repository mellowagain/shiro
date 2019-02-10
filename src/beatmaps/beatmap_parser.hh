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

#ifndef SHIRO_BEATMAP_PARSER_HH
#define SHIRO_BEATMAP_PARSER_HH

#include <cstdint>

#include "../utils/play_mode.hh"
#include "beatmap_hit_object.hh"

namespace shiro::beatmaps {

    // Why does peppy always skip 2
    static constexpr int32_t circle_type = 1 << 0;
    static constexpr int32_t slider_type = 1 << 1;
    static constexpr int32_t spinner_type = 1 << 3;

    class timing_point {
    public:
        double time = 0.0;
        double ms_per_beat = -100.0;

        bool change = false;

    };

    class parser {
    private:
        int32_t version = 0;

        int32_t beatmap_id = 0;
        utils::play_mode mode = utils::play_mode::standard;

        std::string title;
        std::string title_unicode; // .size() and .length() will be in bytes, not the char amount

        std::string artist;
        std::string artist_unicode; // .size() and .length() will be in bytes, not the char amount

        std::string creator;
        std::string difficulty_name;

        void parse_metadata(std::string &line);
        void parse_general(std::string &line);
        void parse_difficulty(std::string &line);
        void parse_timing_points(std::string &line);
        void parse_hit_objects(std::string &line);

    public:
        int32_t circles = 0;
        int32_t sliders = 0;
        int32_t spinners = 0;

        float hp = -1.0f;
        float cs = -1.0f;
        float od = -1.0f;
        float ar = -1.0f;

        float slider_velocity = 1.0f;
        float tick_rate = 1.0f;

        std::vector<hit_object> objects;
        std::vector<timing_point> timing_points;

        explicit parser(int32_t beatmap_id);

        bool parse();

    };

}

#endif //SHIRO_BEATMAP_PARSER_HH
