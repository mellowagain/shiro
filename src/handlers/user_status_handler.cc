#include "../io/layouts/user/user_status.hh"
#include "user_status_handler.hh"

void shiro::handler::user_status::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    io::layouts::user_status status = in.unmarshal<io::layouts::user_status>();

    user->status = status;
}
