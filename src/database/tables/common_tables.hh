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

#ifndef SHIRO_COMMON_TABLES_HH
#define SHIRO_COMMON_TABLES_HH

#define object_detailed_struct(name, var, type)                                     \
    struct var {                                                                    \
        struct _alias_t {                                                           \
            static constexpr const char _literal[] = name;                          \
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;  \
            template <typename T>                                                   \
            struct _member_t {                                                      \
                T var;                                                              \
                T &operator()() {                                                   \
                    return var;                                                     \
                }                                                                   \
                const T &operator()() const {                                       \
                    return var;                                                     \
                }                                                                   \
            };                                                                      \
        };                                                                          \
        using _traits = sqlpp::make_traits<type>;                                   \
    }

#define object_struct(name, type) object_detailed_struct(#name, name, type)

#define database_table(name, ...)                                                   \
    struct name : sqlpp::table_t<name, ##__VA_ARGS__> {                             \
        using _value_type = sqlpp::no_value_t;                                      \
        struct _alias_t {                                                           \
            static constexpr const char _literal[] = #name;                         \
            using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;  \
            template <typename T>                                                   \
            struct _member_t {                                                      \
                T _table;                                                           \
                T &operator()() {                                                   \
                    return _table;                                                  \
                }                                                                   \
                const T &operator()() const {                                       \
                    return _table;                                                  \
                }                                                                   \
            };                                                                      \
        };                                                                          \
    }

// ===================
// User stats specific
// ===================

#define fields                                                                      \
        object_struct(id, sqlpp::integer);                                          \
        object_struct(pp_std, sqlpp::floating_point);                               \
        object_struct(pp_taiko, sqlpp::floating_point);                             \
        object_struct(pp_ctb, sqlpp::floating_point);                               \
        object_struct(pp_mania, sqlpp::floating_point);                             \
        object_struct(rank_std, sqlpp::integer);                                    \
        object_struct(rank_taiko, sqlpp::integer);                                  \
        object_struct(rank_ctb, sqlpp::integer);                                    \
        object_struct(rank_mania, sqlpp::integer);                                  \
        object_struct(score_std, sqlpp::integer);                                   \
        object_struct(score_taiko, sqlpp::integer);                                 \
        object_struct(score_ctb, sqlpp::integer);                                   \
        object_struct(score_mania, sqlpp::integer);                                 \
        object_struct(ranked_score_std, sqlpp::integer);                            \
        object_struct(ranked_score_taiko, sqlpp::integer);                          \
        object_struct(ranked_score_ctb, sqlpp::integer);                            \
        object_struct(ranked_score_mania, sqlpp::integer);                          \
        object_struct(accuracy_std, sqlpp::floating_point);                         \
        object_struct(accuracy_taiko, sqlpp::floating_point);                       \
        object_struct(accuracy_ctb, sqlpp::floating_point);                         \
        object_struct(accuracy_mania, sqlpp::floating_point);                       \
        object_struct(max_combo_std, sqlpp::integer);                               \
        object_struct(max_combo_taiko, sqlpp::integer);                             \
        object_struct(max_combo_ctb, sqlpp::integer);                               \
        object_struct(max_combo_mania, sqlpp::integer);                             \
        object_struct(play_count_std, sqlpp::integer);                              \
        object_struct(play_count_taiko, sqlpp::integer);                            \
        object_struct(play_count_ctb, sqlpp::integer);                              \
        object_struct(play_count_mania, sqlpp::integer);

#define database_stats_table(table, structure)                                      \
        database_table(table,                                                       \
                ##structure::id,                                                    \
                ##structure::pp_std,                                                \
                ##structure::pp_taiko,                                              \
                ##structure::pp_ctb,                                                \
                ##structure::pp_mania,                                              \
                ##structure::rank_std,                                              \
                ##structure::rank_taiko,                                            \
                ##structure::rank_ctb,                                              \
                ##structure::rank_mania,                                            \
                ##structure::score_std,                                             \
                ##structure::score_taiko,                                           \
                ##structure::score_ctb,                                             \
                ##structure::score_mania,                                           \
                ##structure::ranked_score_std,                                      \
                ##structure::ranked_score_taiko,                                    \
                ##structure::ranked_score_ctb,                                      \
                ##structure::ranked_score_mania,                                    \
                ##structure::accuracy_std,                                          \
                ##structure::accuracy_taiko,                                        \
                ##structure::accuracy_ctb,                                          \
                ##structure::accuracy_mania,                                        \
                ##structure::max_combo_std,                                         \
                ##structure::max_combo_taiko,                                       \
                ##structure::max_combo_ctb,                                         \
                ##structure::max_combo_mania,                                       \
                ##structure::play_count_std,                                        \
                ##structure::play_count_taiko,                                      \
                ##structure::play_count_ctb,                                        \
                ##structure::play_count_mania                                       \
    );

#define create_table_stats_str(table)                                                                               \
    "CREATE TABLE IF NOT EXISTS `" table "` "                                                                       \
    "(id INT PRIMARY KEY NOT NULL, "                                                                                \
    "pp_std FLOAT NOT NULL DEFAULT 0, pp_taiko FLOAT NOT NULL DEFAULT 0, pp_ctb FLOAT NOT NULL DEFAULT 0, "         \
    "pp_mania FLOAT NOT NULL DEFAULT 0, rank_std INT NOT NULL DEFAULT 0, rank_taiko INT NOT NULL DEFAULT 0, "       \
    "rank_ctb INT NOT NULL DEFAULT 0, rank_mania INT NOT NULL DEFAULT 0, score_std INT NOT NULL DEFAULT 0, "        \
    "score_taiko INT NOT NULL DEFAULT 0, score_ctb INT NOT NULL DEFAULT 0, score_mania INT NOT NULL DEFAULT 0, "    \
    "ranked_score_std INT NOT NULL DEFAULT 0, ranked_score_taiko INT NOT NULL DEFAULT 0, "                          \
    "ranked_score_ctb INT NOT NULL DEFAULT 0, ranked_score_mania INT NOT NULL DEFAULT 0, "                          \
    "accuracy_std FLOAT NOT NULL DEFAULT 0, accuracy_taiko FLOAT NOT NULL DEFAULT 0, "                              \
    "accuracy_ctb FLOAT NOT NULL DEFAULT 0, accuracy_mania FLOAT NOT NULL DEFAULT 0, "                              \
    "play_count_std INT NOT NULL DEFAULT 0, play_count_taiko INT NOT NULL DEFAULT 0, "                              \
    "play_count_ctb INT NOT NULL DEFAULT 0, play_count_mania INT NOT NULL DEFAULT 0, "                              \
    "max_combo_std INT NOT NULL DEFAULT 0, max_combo_taiko INT NOT NULL DEFAULT 0, "                                \
    "max_combo_ctb INT NOT NULL DEFAULT 0, max_combo_mania INT NOT NULL DEFAULT 0, "                                \
    ");"

#endif  // SHIRO_COMMON_TABLES_HH
