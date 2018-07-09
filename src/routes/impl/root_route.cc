#include "root_route.hh"
#include "../../io/osu_writer.hh"

void shiro::routes::root::handle(const httplib::Request &request, httplib::Response &response) {
    io::osu_writer writer;

    writer.protocol_negotiation(19);
    writer.login_reply(69);
    writer.announce("shiro is epic dude");

    response.set_content(writer.serialize(), "application/octet-stream");
}
