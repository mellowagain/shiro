#include "../../spectating/spectator_manager.hh"
#include "spectator_frames_handler.hh"

void shiro::handler::spectating::frames::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    auto spectators = shiro::spectating::manager::get_spectators(user);
    if (spectators.empty())
        return;

    io::osu_writer writer;
    writer.spectate_frames(in.data);

    for (const std::shared_ptr<shiro::users::user> &spectator : spectators) {
        spectator->queue.enqueue(writer);
    }
}
