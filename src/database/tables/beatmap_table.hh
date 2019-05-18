/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2019 Marc3842h, czapek
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
#include <sqlpp11/mysql/connection.h>
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
        object_struct(size, sqlpp::floating_point);
        object_struct(drain, sqlpp::floating_point);
        object_struct(aim, sqlpp::floating_point);
        object_struct(speed, sqlpp::floating_point);
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

    database_table(beatmaps,
            beatmaps_objects::id,
            beatmaps_objects::beatmap_id,
            beatmaps_objects::beatmapset_id,
            beatmaps_objects::beatmap_md5,
            beatmaps_objects::song_name,
            beatmaps_objects::game_mode,
            beatmaps_objects::ar,
            beatmaps_objects::od,
            beatmaps_objects::size,
            beatmaps_objects::drain,
            beatmaps_objects::aim,
            beatmaps_objects::speed,
            beatmaps_objects::diff_std,
            beatmaps_objects::diff_taiko,
            beatmaps_objects::diff_ctb,
            beatmaps_objects::diff_mania,
            beatmaps_objects::max_combo,
            beatmaps_objects::hit_length,
            beatmaps_objects::bpm,
            beatmaps_objects::ranked_status,
            beatmaps_objects::last_update,
            beatmaps_objects::ranked_status_freezed,
            beatmaps_objects::play_count,
            beatmaps_objects::pass_count
    );

    namespace migrations::beatmaps {

        inline void create(sqlpp::mysql::connection &db) {
            db.execute(
                    "CREATE TABLE IF NOT EXISTS `beatmaps` "
                    "(id INT PRIMARY KEY NOT NULL AUTO_INCREMENT, "
                    "beatmap_id INT NOT NULL, beatmapset_id INT NOT NULL, beatmap_md5 VARCHAR(35) NOT NULL, song_name VARCHAR(128) NOT NULL, "
                    "game_mode SMALLINT NOT NULL, ar FLOAT NOT NULL, od FLOAT NOT NULL, size FLOAT NOT NULL, drain FLOAT NOT NULL, "
                    "aim FLOAT NOT NULL, speed FLOAT NOT NULL, diff_std FLOAT NOT NULL, diff_taiko FLOAT NOT NULL, diff_ctb FLOAT NOT NULL, "
                    "diff_mania FLOAT NOT NULL, max_combo INT NOT NULL, hit_length INT NOT NULL, bpm INT NOT NULL, ranked_status INT NOT NULL, "
                    "last_update INT NOT NULL, ranked_status_freezed BOOLEAN NOT NULL, play_count INT NOT NULL, pass_count INT NOT NULL);"
            );
        }

    }

}

#endif  // SHIRO_BEATMAP_TABLE_HH
