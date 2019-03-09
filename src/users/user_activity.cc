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

#include <chrono>

#include "../database/tables/user_table.hh"
#include "../scores/score.hh"
#include "../scores/score_helper.hh"
#include "../thirdparty/loguru.hh"
#include "../shiro.hh"
#include "user.hh"
#include "user_activity.hh"
#include "user_manager.hh"

void shiro::users::activity::init() {
    scheduler.Schedule(1min, [](tsc::TaskContext ctx) {
        sqlpp::mysql::connection db(db_connection->get_config());
        const tables::users user_table {};

        users::manager::iterate([&db, &user_table](std::shared_ptr<users::user> user) {
            db(update(user_table).set(
                    user_table.last_seen = user->last_ping.count()
            ).where(user_table.id == user->user_id));
        }, true);

        ctx.Repeat();
    });
}

bool shiro::users::activity::is_inactive(int32_t id, const utils::play_mode &mode) {
    using days = std::chrono::duration<int32_t, std::ratio<86400>>;

    std::optional<scores::score> score = scores::helper::get_latest_score(id, mode);

    if (!score.has_value())
        return false;

    scores::score latest_score = score.value();

    std::chrono::duration epoch_time = std::chrono::system_clock::now().time_since_epoch();
    std::chrono::duration score_time = std::chrono::seconds(latest_score.time);
    std::chrono::duration difference_time = epoch_time - score_time;

    int32_t difference = std::chrono::duration_cast<days>(difference_time).count();
    bool inactive = difference > 90; // Users which haven't submitted a score in 90 days are considered inactive

    if (inactive)
        LOG_F(MAX, "User %i is inactive (%i days since last score submission)", id, difference);

    return inactive;
}
