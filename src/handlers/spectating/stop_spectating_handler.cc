#include "../../spectating/spectator_manager.hh"
#include "stop_spectating_handler.hh"

void shiro::handler::spectating::stop::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    if (!shiro::spectating::manager::is_spectating(user))
        return;

    std::shared_ptr<users::user> host = shiro::spectating::manager::get_host(user);

    if (host == nullptr)
        return;

    shiro::spectating::manager::stop_spectating(user);

    if (user->hidden)
        return;

    io::osu_writer host_writer;
    host_writer.spectator_left_host(user->user_id);

    host->queue.enqueue(host_writer);

    io::osu_writer user_writer;
    user_writer.spectator_left_user(user->user_id);

    for (const std::shared_ptr<shiro::users::user> &fellow_spectator : shiro::spectating::manager::get_spectators(host)) {
        fellow_spectator->queue.enqueue(user_writer);
    }
}
