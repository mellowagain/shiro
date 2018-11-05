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

#ifndef SHIRO_SPECTATE_FRAMES_HH
#define SHIRO_SPECTATE_FRAMES_HH

#include <vector>

#include "../serializable.hh"
#include "replay_frame.hh"
#include "score_frame.hh"

namespace shiro::io::layouts {

    class spectate_frames : public serializable {
    public:
        int32_t extra = 0;
        std::vector<replay_frame> replay_frames;
        uint8_t action = 0;
        layouts::score_frame score_frame;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;
    };
}

#endif //SHIRO_SPECTATE_FRAMES_HH
