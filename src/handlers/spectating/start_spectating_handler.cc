#include "../../spectating/spectator_manager.hh"
#include "../../users/user_manager.hh"
#include "start_spectating_handler.hh"

void shiro::handler::spectating::start::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    int32_t target_id = in.data.read<int32_t>();

    std::shared_ptr<shiro::users::user> target_user = users::manager::get_user_by_id(target_id);
    if (target_user == nullptr)
        return;

    io::osu_writer host_writer;
    host_writer.spectator_join_host(user->user_id);

    target_user->queue.enqueue(host_writer);

    io::osu_writer user_writer;
    user_writer.spectator_join_user(user->user_id);

    for (const std::shared_ptr<shiro::users::user> &fellow_spectator : shiro::spectating::manager::get_spectators(target_user)) {
        fellow_spectator->queue.enqueue(user_writer);
    }

    shiro::spectating::manager::start_spectating(user, target_user);
}
