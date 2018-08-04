#ifndef SHIRO_CHANNEL_TABLE_HH
#define SHIRO_CHANNEL_TABLE_HH

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>
#include <sqlpp11/table.h>

#include "common_tables.hh"

namespace shiro::tables {

    struct channel_objects {
        object_struct(id, sqlpp::integer);
        object_struct(name, sqlpp::varchar);
        object_struct(description, sqlpp::varchar);
        object_struct(auto_join, sqlpp::boolean);
    };

    struct channels : sqlpp::table_t<channels, channel_objects::id,
            channel_objects::name, channel_objects::description,
            channel_objects::auto_join> {
        using _value_type = sqlpp::no_value_t;
        struct _alias_t
        {
            static constexpr const char _literal[] = "channels";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t
            {
                T _users;
                T& operator()()
                {
                    return _users;
                }
                const T& operator()() const
                {
                    return _users;
                }
            };
        };
    };

}

#endif  // SHIRO_CHANNEL_TABLE_HH
