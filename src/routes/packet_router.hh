#ifndef SHIRO_PACKET_ROUTER_HH
#define SHIRO_PACKET_ROUTER_HH

#include "../io/layouts/packets.hh"
#include "../io/osu_packet.hh"
#include "../io/osu_writer.hh"
#include "../users/user.hh"

namespace shiro::routes {

    void route(io::packet_id packet_id, io::osu_packet &in, io::osu_writer &out, std::shared_ptr<users::user> user);

}

#endif //SHIRO_PACKET_ROUTER_HH
