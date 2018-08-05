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

#endif  // SHIRO_RELATIONSHIP_TABLE_HH
