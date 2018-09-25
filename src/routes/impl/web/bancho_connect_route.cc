#include "../../../users/user.hh"
#include "bancho_connect_route.hh"

void shiro::routes::web::bancho_connect::handle(const crow::request &request, crow::response &response) {
    response.set_header("Content-Type", "text/plain; charset=UTF-8");
    response.set_header("cho-server", "shiro (https://github.com/Marc3842h/shiro)");

    char *username = request.url_params.get("u");
    char *md5sum = request.url_params.get("h");

    if (username == nullptr || md5sum == nullptr) {
        response.code = 400;
        response.end();
        return;
    }

    std::shared_ptr user = std::make_shared<users::user>(username);

    if (!user->init()) {
        response.code = 400;
        response.end();
        return;
    }

    if (!user->check_password(md5sum)) {
        response.code = 401;
        response.end();
        return;
    }

    std::string country = user->country;
    std::transform(country.begin(), country.end(), country.begin(), ::tolower);

    response.end(country);
}
