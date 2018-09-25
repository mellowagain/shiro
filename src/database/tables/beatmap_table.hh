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

#ifndef SHIRO_BEATMAP_TABLE_HH
#define SHIRO_BEATMAP_TABLE_HH

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>
#include <sqlpp11/table.h>

#include "common_tables.hh"

namespace shiro::tables {

    struct beatmaps_objects {
        object_struct(id, sqlpp::integer);
        object_struct(beatmap_id, sqlpp::integer);
        object_struct(beatmapset_id, sqlpp::integer);
        object_struct(beatmap_md5, sqlpp::varchar);
        object_struct(song_name, sqlpp::varchar);
        object_struct(game_mode, sqlpp::smallint);
        object_struct(ar, sqlpp::floating_point);
        object_struct(od, sqlpp::floating_point);
        object_struct(diff_std, sqlpp::floating_point);
        object_struct(diff_taiko, sqlpp::floating_point);
        object_struct(diff_ctb, sqlpp::floating_point);
        object_struct(diff_mania, sqlpp::floating_point);
        object_struct(max_combo, sqlpp::integer);
        object_struct(hit_length, sqlpp::integer);
        object_struct(bpm, sqlpp::integer);
        object_struct(ranked_status, sqlpp::integer);
        object_struct(last_update, sqlpp::integer);
        object_struct(ranked_status_freezed, sqlpp::boolean);
        object_struct(play_count, sqlpp::integer);
        object_struct(pass_count, sqlpp::integer);
    };

    struct beatmaps : sqlpp::table_t<beatmaps, beatmaps_objects::id, beatmaps_objects::beatmap_id,
            beatmaps_objects::beatmapset_id, beatmaps_objects::beatmap_md5, beatmaps_objects::song_name, beatmaps_objects::game_mode,
            beatmaps_objects::ar, beatmaps_objects::od, beatmaps_objects::diff_std, beatmaps_objects::diff_taiko,
            beatmaps_objects::diff_ctb, beatmaps_objects::diff_mania, beatmaps_objects::max_combo,
            beatmaps_objects::hit_length, beatmaps_objects::bpm, beatmaps_objects::ranked_status, beatmaps_objects::last_update,
            beatmaps_objects::ranked_status_freezed, beatmaps_objects::play_count, beatmaps_objects::pass_count> {
        using _value_type = sqlpp::no_value_t;
        struct _alias_t {
            static constexpr const char _literal[] = "beatmaps";
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
            template <typename T>
            struct _member_t {
                T _beatmaps;
                T &operator()() {
                    return _beatmaps;
                }
                const T &operator()() const {
                    return _beatmaps;
                }
            };
        };
    };

}

#endif  // SHIRO_BEATMAP_TABLE_HH
