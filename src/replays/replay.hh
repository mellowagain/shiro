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

#ifndef SHIRO_REPLAY_HH
#define SHIRO_REPLAY_HH

#include <string>
#include <vector>

#include "../scores/score.hh"

namespace shiro::replays {

    class action {
    public:
        int64_t w = 0;
        float x = 0.0f;
        float y = 0.0f;
        int32_t z = 0; // buttons

        action() = default;

        std::string to_string() const;

    };

    class replay {
    private:
        scores::score score;
        std::vector<action> actions;
        std::string raw_replay;

    public:
        replay(scores::score s, std::string replay);

        void parse();

        std::vector<action> get_actions();

        std::string to_string() const;

    };

}

#endif //SHIRO_REPLAY_HH
