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

#ifndef SHIRO_OPPAI_WRAPPER_HH
#define SHIRO_OPPAI_WRAPPER_HH

#include "../../thirdparty/oppai.hh"
#include "../../beatmaps/beatmap.hh"
#include "../../scores/score.hh"

namespace shiro::pp {

    class oppai_wrapper {
    private:
        ezpp_t ez = nullptr;

    public:
        oppai_wrapper(beatmaps::beatmap beatmap, scores::score score);
        ~oppai_wrapper();

        float calculate();

    };

}

#endif //SHIRO_OPPAI_WRAPPER_HH
