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

#ifndef SHIRO_PYTHON_UTILS_HH
#define SHIRO_PYTHON_UTILS_HH

#include <boost/python.hpp>

namespace shiro::utils::python {

    // In order to be able to call functions from Python, we need a wrapper around
    // common functions from Shiro which we pass to execute()
    class wrapper {
    public:
        void write(std::string message, int32_t user_id, std::string channel);
        void write_global(std::string message, std::string channel);

    };

    // Source: https://stackoverflow.com/a/48970483
    template <typename container>
    boost::python::list to_python_list(const container& vec) {
        using t = typename container::value_type;

        boost::python::list list;

        std::for_each(vec.begin(), vec.end(), [&](const t& t) {
            list.append(t);
        });

        return list;
    }

    void init();

}

namespace boost::python {

    object reload(boost::python::object object);

}

#endif //SHIRO_PYTHON_UTILS_HH
