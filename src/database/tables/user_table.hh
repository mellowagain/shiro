#ifndef SHIRO_USER_TABLE_HH
#define SHIRO_USER_TABLE_HH

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>
#include <sqlpp11/table.h>

#include "common_tables.hh"

namespace shiro::tables {

    struct users_objects {
        object_struct(id, sqlpp::integer);
        object_struct(username, sqlpp::varchar);
        object_struct(safe_username, sqlpp::varchar);
        object_struct(password, sqlpp::varchar);
        object_struct(salt, sqlpp::varchar);
        object_struct(email, sqlpp::varchar);
        object_struct(ip, sqlpp::varchar);
        object_struct(registration_date, sqlpp::integer);
        object_struct(last_seen, sqlpp::integer);
        object_struct(followers, sqlpp::integer);
        object_struct(groups, sqlpp::integer);
        object_struct(user_page, sqlpp::text);
        object_struct(pp_std, sqlpp::floating_point);
        object_struct(pp_taiko, sqlpp::floating_point);
        object_struct(pp_ctb, sqlpp::floating_point);
        object_struct(pp_mania, sqlpp::floating_point);
        object_struct(score_std, sqlpp::integer);
        object_struct(score_taiko, sqlpp::integer);
        object_struct(score_ctb, sqlpp::integer);
        object_struct(score_mania, sqlpp::integer);
        object_struct(ranked_score_std, sqlpp::integer);
        object_struct(ranked_score_taiko, sqlpp::integer);
        object_struct(ranked_score_ctb, sqlpp::integer);
        object_struct(ranked_score_mania, sqlpp::integer);
        object_struct(play_count_std, sqlpp::integer);
        object_struct(play_count_taiko, sqlpp::integer);
        object_struct(play_count_ctb, sqlpp::integer);
        object_struct(play_count_mania, sqlpp::integer);
        object_struct(country, sqlpp::varchar);
    };

    struct users : sqlpp::table_t<users, users_objects::id, users_objects::username, users_objects::safe_username,
            users_objects::password, users_objects::salt, users_objects::email, users_objects::ip,
            users_objects::registration_date, users_objects::last_seen,
            users_objects::followers, users_objects::groups, users_objects::user_page,
            users_objects::pp_std, users_objects::pp_taiko, users_objects::pp_ctb, users_objects::pp_mania,
            users_objects::score_std, users_objects::score_taiko, users_objects::score_ctb, users_objects::score_mania,
            users_objects::ranked_score_std, users_objects::ranked_score_taiko, users_objects::ranked_score_ctb, users_objects::ranked_score_mania,
            users_objects::play_count_std, users_objects::play_count_taiko, users_objects::play_count_ctb, users_objects::play_count_mania,
            users_objects::country> {
        using _value_type = sqlpp::no_value_t;
        struct _alias_t {
            static constexpr const char _literal[] = "users";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t {
                T _users;
                T &operator()() {
                    return _users;
                }
                const T &operator()() const {
                    return _users;
                }
            };
        };
    };

}

#endif  // SHIRO_USER_TABLE_HH
