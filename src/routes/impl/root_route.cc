#include "../../io/osu_writer.hh"
#include "root_route.hh"

void shiro::routes::root::handle(const crow::request &request, crow::response &response) {
    if (request.get_header_value("user-agent").find("osu!") != std::string::npos) {
        io::osu_writer writer;

        response.set_header("cho-protocol", "19");
        response.set_header("Connection", "keep-alive");
        response.set_header("Keep-Alive", "timeout=5, max=100");
        response.set_header("Content-Type", "text/html; charset=UTF-8");

        if (request.get_header_value("osu-token").length() > 0) {
            // handle some packets
        } else {
            writer.protocol_negotiation(19);
            writer.login_reply(69);
            writer.announce("shiro is epic dude");

            response.set_header("cho-token", "NIGGERS");
            response.end(writer.serialize());
        }
    }
}
