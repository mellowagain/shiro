#ifndef SHIRO_USER_STATS_TABLE_HH
#define SHIRO_USER_STATS_TABLE_HH

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/column_types.h>
#include <sqlpp11/mysql/connection.h>
#include <sqlpp11/table.h>

#include "common_tables.hh"

namespace shiro::tables {

    struct user_stats_objects {
        fields
    };

    database_stats_table(user_stats, user_stats_objects)

    #if defined(SEPARATE_RX_LEADERBOARDS)
        struct user_rx_stats_objects {
            fields
        };

        database_stats_table(user_rx_stats, user_rx_stats_objects)
    #endif

    #if defined(SEPARATE_AP_LEADERBOARDS)
        struct user_ap_stats_objects {
            fields
        };

        database_stats_table(user_ap_stats, user_ap_stats_objects)
    #endif

    namespace migrations::user_stats {

        inline void create(sqlpp::mysql::connection &db) {
            db.execute(create_table_stats_str("user_stats"));

            #if defined(SEPARATE_RX_LEADERBOARDS)
                db.execute(create_table_stats_str("user_rx_stats"));
            #endif

            #if defined(SEPARATE_AP_LEADERBOARDS)
                db.execute(create_table_stats_str("user_ap_stats"));
            #endif
        }

    }

}

#endif  // SHIRO_USER_STATS_TABLE_HH
