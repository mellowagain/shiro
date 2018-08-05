#include <vector>

#include "../../users/user_manager.hh"
#include "user_presence_request_handler.hh"

void shiro::handler::presence::request_all::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    std::vector<int32_t> requested_users = in.data.read_array();

    for (int32_t requested_user : requested_users) {
        if (!users::manager::is_online(requested_user))
            continue;

        std::shared_ptr<users::user> target_user = users::manager::get_user_by_id(requested_user);

        out.user_presence(target_user->presence);
        out.user_stats(target_user->stats);
    }

    std::vector<int32_t> online_users;
    online_users.reserve(users::manager::online_users.size());

    for (const std::shared_ptr<users::user> &online_user : users::manager::online_users) {
        online_users.emplace_back(online_user->user_id);
    }

    out.users_list(online_users);
}
