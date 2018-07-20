#include <chrono>

#include "ping_handler.hh"

void shiro::handler::ping::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );
    user->last_ping = seconds;

    out.user_presence(user->presence);
    out.user_stats(user->stats);
}
