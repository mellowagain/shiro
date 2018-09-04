#include "../../io/layouts/replay/spectate_frames.hh"
#include "../../spectating/spectator_manager.hh"
#include "spectator_frames_handler.hh"

#include "../../thirdparty/loguru.hh"

void shiro::handler::spectating::frames::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    auto spectators = shiro::spectating::manager::get_spectators(user);

    if (spectators.empty())
        return;

    io::layouts::spectate_frames frames = in.unmarshal<io::layouts::spectate_frames>();

    io::osu_writer writer;
    writer.spectate_frames(frames);

    for (const std::shared_ptr<users::user> &spectator : spectators) {
        spectator->queue.enqueue(writer);
    }
}
