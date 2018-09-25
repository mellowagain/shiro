#include <chrono>

#include "../database/tables/user_table.hh"
#include "../shiro.hh"
#include "user.hh"
#include "user_activity.hh"
#include "user_manager.hh"

void shiro::users::activity::init() {
    scheduler.Schedule(1min, [&](tsc::TaskContext ctx) {
        sqlpp::mysql::connection db(db_connection->get_config());
        const tables::users user_table {};

        for (const std::shared_ptr<users::user> &user : users::manager::online_users) {
            if (user == nullptr || user->user_id == 1)
                continue;

            db(update(user_table).set(
                user_table.last_seen = user->last_ping.count()
            ).where(user_table.id == user->user_id));
        }

        ctx.Repeat();
    });
}
