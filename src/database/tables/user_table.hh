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

#ifndef SHIRO_USER_TABLE_HH
#define SHIRO_USER_TABLE_HH

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>
#include <sqlpp11/mysql/connection.h>
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
        object_struct(roles, sqlpp::integer_unsigned);
        object_struct(user_page, sqlpp::text);
        object_struct(pp_std, sqlpp::floating_point);
        object_struct(pp_taiko, sqlpp::floating_point);
        object_struct(pp_ctb, sqlpp::floating_point);
        object_struct(pp_mania, sqlpp::floating_point);
        object_struct(rank_std, sqlpp::integer);
        object_struct(rank_taiko, sqlpp::integer);
        object_struct(rank_ctb, sqlpp::integer);
        object_struct(rank_mania, sqlpp::integer);
        object_struct(score_std, sqlpp::integer);
        object_struct(score_taiko, sqlpp::integer);
        object_struct(score_ctb, sqlpp::integer);
        object_struct(score_mania, sqlpp::integer);
        object_struct(ranked_score_std, sqlpp::integer);
        object_struct(ranked_score_taiko, sqlpp::integer);
        object_struct(ranked_score_ctb, sqlpp::integer);
        object_struct(ranked_score_mania, sqlpp::integer);
        object_struct(accuracy_std, sqlpp::floating_point);
        object_struct(accuracy_taiko, sqlpp::floating_point);
        object_struct(accuracy_ctb, sqlpp::floating_point);
        object_struct(accuracy_mania, sqlpp::floating_point);
        object_struct(max_combo_std, sqlpp::integer);
        object_struct(max_combo_taiko, sqlpp::integer);
        object_struct(max_combo_ctb, sqlpp::integer);
        object_struct(max_combo_mania, sqlpp::integer);
        object_struct(play_count_std, sqlpp::integer);
        object_struct(play_count_taiko, sqlpp::integer);
        object_struct(play_count_ctb, sqlpp::integer);
        object_struct(play_count_mania, sqlpp::integer);
        object_struct(country, sqlpp::varchar);
    };

    database_table(users,
            users_objects::id,
            users_objects::username,
            users_objects::safe_username,
            users_objects::password,
            users_objects::salt,
            users_objects::email,
            users_objects::ip,
            users_objects::registration_date,
            users_objects::last_seen,
            users_objects::followers,
            users_objects::roles,
            users_objects::user_page,
            users_objects::pp_std,
            users_objects::pp_taiko,
            users_objects::pp_ctb,
            users_objects::pp_mania,
            users_objects::rank_std,
            users_objects::rank_taiko,
            users_objects::rank_ctb,
            users_objects::rank_mania,
            users_objects::score_std,
            users_objects::score_taiko,
            users_objects::score_ctb,
            users_objects::score_mania,
            users_objects::ranked_score_std,
            users_objects::ranked_score_taiko,
            users_objects::ranked_score_ctb,
            users_objects::ranked_score_mania,
            users_objects::accuracy_std,
            users_objects::accuracy_taiko,
            users_objects::accuracy_ctb,
            users_objects::accuracy_mania,
            users_objects::max_combo_std,
            users_objects::max_combo_taiko,
            users_objects::max_combo_ctb,
            users_objects::max_combo_mania,
            users_objects::play_count_std,
            users_objects::play_count_taiko,
            users_objects::play_count_ctb,
            users_objects::play_count_mania,
            users_objects::country
    );

    namespace migrations::users {

        inline void create(sqlpp::mysql::connection &db) {
            db.execute(
                    "CREATE TABLE IF NOT EXISTS `users` "
                    "(id INT PRIMARY KEY NOT NULL AUTO_INCREMENT, "
                    "username VARCHAR(32) NOT NULL, safe_username VARCHAR(32) NOT NULL, "
                    "password VARCHAR(128) NOT NULL, salt VARCHAR(64) NOT NULL,"
                    "email VARCHAR(100) NOT NULL, ip VARCHAR(39) NOT NULL, registration_date INT NOT NULL, last_seen INT NOT NULL, "
                    "followers INT NOT NULL, roles INT UNSIGNED NOT NULL, user_page TEXT NOT NULL, "
                    "pp_std FLOAT NOT NULL DEFAULT 0, pp_taiko FLOAT NOT NULL DEFAULT 0, pp_ctb FLOAT NOT NULL DEFAULT 0, "
                    "pp_mania FLOAT NOT NULL DEFAULT 0, rank_std INT NOT NULL DEFAULT 0, rank_taiko INT NOT NULL DEFAULT 0, "
                    "rank_ctb INT NOT NULL DEFAULT 0, rank_mania INT NOT NULL DEFAULT 0, score_std INT NOT NULL DEFAULT 0, "
                    "score_taiko INT NOT NULL DEFAULT 0, score_ctb INT NOT NULL DEFAULT 0, score_mania INT NOT NULL DEFAULT 0, "
                    "ranked_score_std INT NOT NULL DEFAULT 0, ranked_score_taiko INT NOT NULL DEFAULT 0, "
                    "ranked_score_ctb INT NOT NULL DEFAULT 0, ranked_score_mania INT NOT NULL DEFAULT 0, "
                    "accuracy_std FLOAT NOT NULL DEFAULT 0, accuracy_taiko FLOAT NOT NULL DEFAULT 0, accuracy_ctb FLOAT NOT NULL DEFAULT 0, "
                    "accuracy_mania FLOAT NOT NULL DEFAULT 0, play_count_std INT NOT NULL DEFAULT 0, play_count_taiko INT NOT NULL DEFAULT 0, "
                    "play_count_ctb INT NOT NULL DEFAULT 0, play_count_mania INT NOT NULL DEFAULT 0, max_combo_std INT NOT NULL DEFAULT 0, "
                    "max_combo_taiko INT NOT NULL DEFAULT 0, max_combo_ctb INT NOT NULL DEFAULT 0, max_combo_mania INT NOT NULL DEFAULT 0, "
                    "country VARCHAR(2) NOT NULL);"
            );
        }

    }

}

#endif  // SHIRO_USER_TABLE_HH
