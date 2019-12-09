#ifndef SHIRO_USER_STATS_TABLE_HH
#define SHIRO_USER_STATS_TABLE_HH

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>
#include <sqlpp11/mysql/connection.h>
#include <sqlpp11/table.h>

#include "common_tables.hh"

namespace shiro::tables {

    struct user_stats_objects {
        object_struct(id, sqlpp::integer);
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
    };

    database_table(user_stats,
            user_stats_objects::id,
            user_stats_objects::pp_std,
            user_stats_objects::pp_taiko,
            user_stats_objects::pp_ctb,
            user_stats_objects::pp_mania,
            user_stats_objects::rank_std,
            user_stats_objects::rank_taiko,
            user_stats_objects::rank_ctb,
            user_stats_objects::rank_mania,
            user_stats_objects::score_std,
            user_stats_objects::score_taiko,
            user_stats_objects::score_ctb,
            user_stats_objects::score_mania,
            user_stats_objects::ranked_score_std,
            user_stats_objects::ranked_score_taiko,
            user_stats_objects::ranked_score_ctb,
            user_stats_objects::ranked_score_mania,
            user_stats_objects::accuracy_std,
            user_stats_objects::accuracy_taiko,
            user_stats_objects::accuracy_ctb,
            user_stats_objects::accuracy_mania,
            user_stats_objects::max_combo_std,
            user_stats_objects::max_combo_taiko,
            user_stats_objects::max_combo_ctb,
            user_stats_objects::max_combo_mania,
            user_stats_objects::play_count_std,
            user_stats_objects::play_count_taiko,
            user_stats_objects::play_count_ctb,
            user_stats_objects::play_count_mania
    );

    #if defined(SEPARATE_RX_LEADERBOARDS)
        database_table(user_stats_rx,
                user_stats_objects::id,
                user_stats_objects::pp_std,
                user_stats_objects::pp_taiko,
                user_stats_objects::pp_ctb,
                user_stats_objects::pp_mania,
                user_stats_objects::rank_std,
                user_stats_objects::rank_taiko,
                user_stats_objects::rank_ctb,
                user_stats_objects::rank_mania,
                user_stats_objects::score_std,
                user_stats_objects::score_taiko,
                user_stats_objects::score_ctb,
                user_stats_objects::score_mania,
                user_stats_objects::ranked_score_std,
                user_stats_objects::ranked_score_taiko,
                user_stats_objects::ranked_score_ctb,
                user_stats_objects::ranked_score_mania,
                user_stats_objects::accuracy_std,
                user_stats_objects::accuracy_taiko,
                user_stats_objects::accuracy_ctb,
                user_stats_objects::accuracy_mania,
                user_stats_objects::max_combo_std,
                user_stats_objects::max_combo_taiko,
                user_stats_objects::max_combo_ctb,
                user_stats_objects::max_combo_mania,
                user_stats_objects::play_count_std,
                user_stats_objects::play_count_taiko,
                user_stats_objects::play_count_ctb,
                user_stats_objects::play_count_mania
        );
    #endif

    #if defined(SEPARATE_AP_LEADERBOARDS)
        database_table(user_stats_ap,
                user_stats_objects::id,
                user_stats_objects::pp_std,
                user_stats_objects::pp_taiko,
                user_stats_objects::pp_ctb,
                user_stats_objects::pp_mania,
                user_stats_objects::rank_std,
                user_stats_objects::rank_taiko,
                user_stats_objects::rank_ctb,
                user_stats_objects::rank_mania,
                user_stats_objects::score_std,
                user_stats_objects::score_taiko,
                user_stats_objects::score_ctb,
                user_stats_objects::score_mania,
                user_stats_objects::ranked_score_std,
                user_stats_objects::ranked_score_taiko,
                user_stats_objects::ranked_score_ctb,
                user_stats_objects::ranked_score_mania,
                user_stats_objects::accuracy_std,
                user_stats_objects::accuracy_taiko,
                user_stats_objects::accuracy_ctb,
                user_stats_objects::accuracy_mania,
                user_stats_objects::max_combo_std,
                user_stats_objects::max_combo_taiko,
                user_stats_objects::max_combo_ctb,
                user_stats_objects::max_combo_mania,
                user_stats_objects::play_count_std,
                user_stats_objects::play_count_taiko,
                user_stats_objects::play_count_ctb,
                user_stats_objects::play_count_mania
        );
    #endif

    #define create_table_stats_str(table)                                                                              \
        "CREATE TABLE IF NOT EXISTS `" table "` "                                                                      \
        "(id INT PRIMARY KEY NOT NULL, "                                                                               \
        "pp_std FLOAT NOT NULL DEFAULT 0, pp_taiko FLOAT NOT NULL DEFAULT 0, pp_ctb FLOAT NOT NULL DEFAULT 0, "        \
        "pp_mania FLOAT NOT NULL DEFAULT 0, rank_std INT NOT NULL DEFAULT 0, rank_taiko INT NOT NULL DEFAULT 0, "      \
        "rank_ctb INT NOT NULL DEFAULT 0, rank_mania INT NOT NULL DEFAULT 0, score_std INT NOT NULL DEFAULT 0, "       \
        "score_taiko INT NOT NULL DEFAULT 0, score_ctb INT NOT NULL DEFAULT 0, score_mania INT NOT NULL DEFAULT 0, "   \
        "ranked_score_std INT NOT NULL DEFAULT 0, ranked_score_taiko INT NOT NULL DEFAULT 0, "                         \
        "ranked_score_ctb INT NOT NULL DEFAULT 0, ranked_score_mania INT NOT NULL DEFAULT 0, "                         \
        "accuracy_std FLOAT NOT NULL DEFAULT 0, accuracy_taiko FLOAT NOT NULL DEFAULT 0, "                             \
        "accuracy_ctb FLOAT NOT NULL DEFAULT 0, accuracy_mania FLOAT NOT NULL DEFAULT 0, "                             \
        "max_combo_std INT NOT NULL DEFAULT 0, max_combo_taiko INT NOT NULL DEFAULT 0, "                               \
        "max_combo_ctb INT NOT NULL DEFAULT 0, max_combo_mania INT NOT NULL DEFAULT 0, "                               \
        "play_count_std INT NOT NULL DEFAULT 0, play_count_taiko INT NOT NULL DEFAULT 0, "                             \
        "play_count_ctb INT NOT NULL DEFAULT 0, play_count_mania INT NOT NULL DEFAULT 0 "                             \
        ");"

    namespace migrations::user_stats {

        inline void create(sqlpp::mysql::connection &db) {
            db.execute(create_table_stats_str("user_stats"));

            #if defined(SEPARATE_RX_LEADERBOARDS)
                db.execute(create_table_stats_str("user_stats_rx"));
            #endif

            #if defined(SEPARATE_AP_LEADERBOARDS)
                db.execute(create_table_stats_str("user_stats_ap"));
            #endif
        }

    }

    #undef create_table_stats_str

}

#endif  // SHIRO_USER_STATS_TABLE_HH
