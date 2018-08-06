#ifndef SHIRO_CANT_SPECTATE_HANDLER_HH
#define SHIRO_CANT_SPECTATE_HANDLER_HH

#include "../../io/osu_packet.hh"
#include "../../io/osu_writer.hh"
#include "../../users/user.hh"

namespace shiro::handler::spectating::cant_spectate {

    void handle(io::osu_packet &in, io::osu_writer &out, std::shared_ptr<users::user> user);

}

#endif //SHIRO_CANT_SPECTATE_HANDLER_HH
