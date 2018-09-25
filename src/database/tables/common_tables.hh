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

#ifndef SHIRO_COMMON_TABLES_HH
#define SHIRO_COMMON_TABLES_HH

#define object_detailed_struct(name, var, type)                             \
struct var {                                                               \
struct _alias_t {                                                           \
    static constexpr const char _literal[] = name;                         \
    using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;  \
    template <typename T>                                                   \
    struct _member_t {                                                      \
        T var;                                                             \
        T &operator()() {                                                   \
            return var;                                                    \
        }                                                                   \
        const T &operator()() const {                                       \
            return var;                                                    \
        }                                                                   \
    };                                                                      \
};                                                                          \
using _traits = sqlpp::make_traits<type>;                                   \
}                                                                           \

#define object_struct(name, type) object_detailed_struct(#name, name, type)

#endif  // SHIRO_COMMON_TABLES_HH
