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

#ifndef SHIRO_VECTOR_UTILS_HH
#define SHIRO_VECTOR_UTILS_HH

#include <cstdint>
#include <vector>

namespace shiro::utils {

    template <typename It>
    std::vector<std::pair<It, It>> chunk(It range_from, It range_to, const ptrdiff_t num) {
        const ptrdiff_t total { std::distance(range_from, range_to) };
        const ptrdiff_t portion { total / num };

        std::vector<std::pair<It,It>> chunks(num);

        It portion_end { range_from };

        std::generate(std::begin(chunks), std::end(chunks), [&portion_end, portion]() {
            It portion_start { portion_end };

            portion_end += portion;
            return std::make_pair(portion_start, portion_end);
        });

        chunks.back().second = range_to;

        return chunks;
    }

}

#endif  // SHIRO_VECTOR_UTILS_HH
