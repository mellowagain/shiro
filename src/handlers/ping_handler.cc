#include <chrono>

#include "ping_handler.hh"

void shiro::handler::ping::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<users::user> user) {
    out.user_presence(user->presence);
    out.user_stats(user->stats);
}
