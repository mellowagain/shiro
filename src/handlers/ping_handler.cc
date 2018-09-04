#include <chrono>

#include "../spectating/spectator_manager.hh"
#include "ping_handler.hh"

void shiro::handler::ping::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    out.user_presence(user->presence);
    out.user_stats(user->stats);

    if (spectating::manager::is_spectating(user)) {
        std::shared_ptr<users::user> host = spectating::manager::get_host(user);

        if (host == nullptr)
            return;

        out.user_presence(host->presence);
        out.user_stats(host->stats);
    }
}
