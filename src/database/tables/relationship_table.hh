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

#ifndef SHIRO_RELATIONSHIP_TABLE_HH
#define SHIRO_RELATIONSHIP_TABLE_HH

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>
#include <sqlpp11/table.h>

#include "common_tables.hh"

namespace shiro::tables {

    struct relationships_objects {
        object_struct(origin, sqlpp::integer);
        object_struct(target, sqlpp::integer);
        object_struct(blocked, sqlpp::boolean);
    };

    struct relationships : sqlpp::table_t<relationships,
            relationships_objects::origin, relationships_objects::target,
            relationships_objects::blocked> {
        using _value_type = sqlpp::no_value_t;
        struct _alias_t {
            static constexpr const char _literal[] = "relationships";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t {
                T _relationships;
                T &operator()() {
                    return _relationships;
                }
                const T &operator()() const {
                    return _relationships;
                }
            };
        };
    };

}

#endif // SHIRO_RELATIONSHIP_TABLE_HH
