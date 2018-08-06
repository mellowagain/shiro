#ifndef SHIRO_START_SPECTATING_HANDLER_HH
#define SHIRO_START_SPECTATING_HANDLER_HH

#include "../../io/osu_packet.hh"
#include "../../io/osu_writer.hh"
#include "../../users/user.hh"

namespace shiro::handler::spectating::start {

    void handle(io::osu_packet &in, io::osu_writer &out, std::shared_ptr<users::user> user);

}

#endif //SHIRO_START_SPECTATING_HANDLER_HH
