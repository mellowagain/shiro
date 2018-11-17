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
        object_struct(ranking, sqlpp::varchar);
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
        object_struct(times_watched, sqlpp::integer);
    };

    struct scores : sqlpp::table_t<scores, scores_objects::id, scores_objects::hash,
            scores_objects::beatmap_md5, scores_objects::user_id, scores_objects::ranking, scores_objects::score, scores_objects::max_combo,
            scores_objects::fc, scores_objects::mods, scores_objects::_300_count, scores_objects::_100_count,
            scores_objects::_50_count, scores_objects::katus_count, scores_objects::gekis_count, scores_objects::miss_count,
            scores_objects::time, scores_objects::play_mode, scores_objects::passed, scores_objects::accuracy,
            scores_objects::pp, scores_objects::times_watched> {
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

#endif // SHIRO_SCORE_TABLE_HH
