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

#ifndef SHIRO_ROLES_TABLE_HH
#define SHIRO_ROLES_TABLE_HH

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>
#include <sqlpp11/table.h>

#include "common_tables.hh"

namespace shiro::tables {

    struct roles_objects {
        object_struct(id, sqlpp::integer_unsigned);
        object_struct(name, sqlpp::varchar);
        object_struct(permissions, sqlpp::bigint_unsigned);
        object_struct(color, sqlpp::tinyint_unsigned);
    };

    struct roles : sqlpp::table_t<roles, roles_objects::id, roles_objects::name, roles_objects::permissions, roles_objects::color> {
        using _value_type = sqlpp::no_value_t;
        struct _alias_t {
            static constexpr const char _literal[] = "roles";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t {
                T _roles;
                T &operator()() {
                    return _roles;
                }
                const T &operator()() const {
                    return _roles;
                }
            };
        };
    };
}

#endif // SHIRO_ROLES_TABLE_HH
