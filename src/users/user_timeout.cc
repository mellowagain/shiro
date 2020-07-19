/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2020 Marc3842h, czapek
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

#include "../thirdparty/loguru.hh"
#include "../shiro.hh"
#include "user_timeout.hh"
#include "user_manager.hh"

void shiro::users::timeout::init() {
    scheduler.Schedule(60s, [](tsc::TaskContext ctx) {
        std::vector<std::shared_ptr<users::user>> dead;
        std::chrono::seconds now = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
        );

        // Iterate over all users to find out which ones are dead
        users::manager::iterate([now, &dead](std::shared_ptr<users::user> user) {
            int64_t difference = std::chrono::duration_cast<std::chrono::seconds>(now - user->last_ping).count();

            if (difference < 60)
                return;

            dead.push_back(user);
        }, true);

        io::osu_writer writer;
        io::layouts::user_quit quit;

        quit.state = 0;

        for (const std::shared_ptr<users::user> &user : dead) {
            LOG_F(WARNING, "User %s didn't send a ping in 60 seconds, timing out.", user->presence.username.c_str());
            users::manager::logout_user(user);

            quit.user_id = user->user_id;

            writer.user_quit(quit);
        }

        users::manager::iterate([&writer](std::shared_ptr<users::user> user) {
            user->queue.enqueue(writer);
        }, true);

        ctx.Repeat();
    });
}
