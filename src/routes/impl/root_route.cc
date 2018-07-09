#include "root_route.hh"

void shiro::routes::root::handle(const httplib::Request &request, httplib::Response &response) {
    response.set_content("Welcome to shiro.", "text/plain");
}
