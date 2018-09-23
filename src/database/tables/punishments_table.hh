#ifndef SHIRO_PUNISHMENTS_TABLE_HH
#define SHIRO_PUNISHMENTS_TABLE_HH

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>
#include <sqlpp11/table.h>

#include "common_tables.hh"

namespace shiro::tables {

    struct punishments_objects {
        object_struct(id, sqlpp::integer);
        object_struct(user_id, sqlpp::integer);
        object_struct(type, sqlpp::tinyint_unsigned);
        object_struct(time, sqlpp::integer);
        object_struct(duration, sqlpp::integer);
        object_struct(active, sqlpp::boolean);
        object_struct(reason, sqlpp::varchar);
    };

    struct punishments : sqlpp::table_t<punishments,
            punishments_objects::id, punishments_objects::user_id,
            punishments_objects::type, punishments_objects::time,
            punishments_objects::duration, punishments_objects::active,
            punishments_objects::reason> {
        using _value_type = sqlpp::no_value_t;
        struct _alias_t {
            static constexpr const char _literal[] = "punishments";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t {
                T _punishments;
                T &operator()() {
                    return _punishments;
                }
                const T &operator()() const {
                    return _punishments;
                }
            };
        };
    };

}

#endif  // SHIRO_PUNISHMENTS_TABLE_HH
