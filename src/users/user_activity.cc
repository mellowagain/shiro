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

#include <chrono>

#include "../database/tables/user_table.hh"
#include "../shiro.hh"
#include "user.hh"
#include "user_activity.hh"
#include "user_manager.hh"

void shiro::users::activity::init() {
    scheduler.Schedule(1min, [](tsc::TaskContext ctx) {
        sqlpp::mysql::connection db(db_connection->get_config());
        const tables::users user_table{};

        for (const std::shared_ptr<users::user> &user : users::manager::online_users) {
            if (user == nullptr || user->user_id == 1)
                continue;

            db(update(user_table)
                    .set(
                        user_table.last_seen = user->last_ping.count())
                    .where(user_table.id == user->user_id));
        }

        ctx.Repeat();
    });
}
