#include <chrono>

#include "../thirdparty/loguru.hh"
#include "../shiro.hh"
#include "user_timeout.hh"
#include "user_manager.hh"

void shiro::users::timeout::init() {
    scheduler.Schedule(30s, [&](tsc::TaskContext ctx) {
        std::vector<io::osu_writer> logout_queue;

        for (const std::shared_ptr<users::user> &user : users::manager::online_users) {
            if (user == nullptr || user->user_id == 1)
                continue;

            std::chrono::seconds now = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch()
            );

            if (std::chrono::duration_cast<std::chrono::seconds>(now - user->last_ping).count() >= 30) {
                LOG_F(WARNING, "User %s didn't send a ping in 30 seconds, timing out.", user->presence.username.c_str());
                users::manager::logout_user(user);

                io::layouts::user_quit quit;
                io::osu_writer writer;

                quit.user_id = user->user_id;
                quit.state = 0;

                writer.user_quit(quit);

                logout_queue.emplace_back(writer);
            }
        }

        for (const std::shared_ptr<users::user> &user : users::manager::online_users) {
            for (io::osu_writer writer : logout_queue) {
                if (user == nullptr || user->user_id == 1)
                    continue;

                user->queue.enqueue(writer);
            }
        }

        ctx.Repeat();
    });
}
