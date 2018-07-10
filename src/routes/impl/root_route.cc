#include "root_route.hh"
#include "../../io/osu_writer.hh"

crow::response shiro::routes::root::handle() {
    io::osu_writer writer;

    writer.protocol_negotiation(19);
    writer.login_reply(69);
    writer.announce("shiro is epic dude");

    crow::response response;

    response.set_header("cho-protocol", "19");
    response.set_header("cho-token", "NIGGERS");
    response.set_header("Connection", "keep-alive");
    response.set_header("Keep-Alive", "timeout=5, max=100");
    response.set_header("Content-Type", "text/html; charset=UTF-8");
    response.write(writer.serialize());

    return response;
}
