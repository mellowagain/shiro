#ifndef SHIRO_SCORE_TABLE_HH
#define SHIRO_SCORE_TABLE_HH

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>
#include <sqlpp11/table.h>

#include "common_tables.hh"

namespace shiro::tables {

    struct scores_objects {
        object_struct(id, sqlpp::integer);
        object_struct(hash, sqlpp::varchar);
        object_struct(beatmap_md5, sqlpp::varchar);
        object_struct(user_id, sqlpp::integer);
        object_struct(rank, sqlpp::varchar);
        object_struct(score, sqlpp::bigint);
        object_struct(max_combo, sqlpp::integer);
        object_struct(fc, sqlpp::boolean);
        object_struct(mods, sqlpp::integer);
        object_detailed_struct("300_count", _300_count, sqlpp::integer);
        object_detailed_struct("100_count", _100_count, sqlpp::integer);
        object_detailed_struct("50_count", _50_count, sqlpp::integer);
        object_struct(katus_count, sqlpp::integer);
        object_struct(gekis_count, sqlpp::integer);
        object_struct(miss_count, sqlpp::integer);
        object_struct(time, sqlpp::integer);
        object_struct(play_mode, sqlpp::tinyint);
        object_struct(passed, sqlpp::boolean);
        object_struct(accuracy, sqlpp::floating_point);
        object_struct(pp, sqlpp::floating_point);
    };

    struct scores : sqlpp::table_t<scores, scores_objects::id, scores_objects::hash,
            scores_objects::beatmap_md5, scores_objects::user_id, scores_objects::rank, scores_objects::score, scores_objects::max_combo,
            scores_objects::fc, scores_objects::mods, scores_objects::_300_count, scores_objects::_100_count,
            scores_objects::_50_count, scores_objects::katus_count, scores_objects::gekis_count, scores_objects::miss_count,
            scores_objects::time, scores_objects::play_mode, scores_objects::passed, scores_objects::accuracy,
            scores_objects::pp> {
        using _value_type = sqlpp::no_value_t;
        struct _alias_t {
            static constexpr const char _literal[] = "scores";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t {
                T _scores;
                T &operator()() {
                    return _scores;
                }
                const T &operator()() const {
                    return _scores;
                }
            };
        };
    };

}

#endif  // SHIRO_SCORE_TABLE_HH
