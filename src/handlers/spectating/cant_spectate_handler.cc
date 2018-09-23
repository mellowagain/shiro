#include "../../spectating/spectator_manager.hh"
#include "cant_spectate_handler.hh"

void shiro::handler::spectating::cant_spectate::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    if (!shiro::spectating::manager::is_spectating(user))
        return;

    std::shared_ptr<users::user> host = shiro::spectating::manager::get_host(user);
    if (host == nullptr)
        return;

    if (host->hidden)
        return;

    io::osu_writer writer;
    writer.spectator_cant_spectate(user->user_id);

    host->queue.enqueue(writer);

    for (const std::shared_ptr<shiro::users::user> &fellow_spectator : shiro::spectating::manager::get_spectators(host)) {
        fellow_spectator->queue.enqueue(writer);
    }
}
