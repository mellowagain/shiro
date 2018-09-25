#include "../users/user_manager.hh"
#include "logout_handler.hh"

void shiro::handler::logout::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    if (!users::manager::is_online(user))
        return;

    users::manager::logout_user(user);

    if (user->hidden)
        return;

    io::layouts::user_quit quit;
    io::osu_writer writer;

    quit.user_id = user->user_id;
    quit.state = 0;

    writer.user_quit(quit);

    for (const std::shared_ptr<users::user> &online_user : users::manager::online_users) {
        if (online_user->user_id == user->user_id || online_user->user_id == 1)
            continue;

        online_user->queue.enqueue(writer);
    }
}
