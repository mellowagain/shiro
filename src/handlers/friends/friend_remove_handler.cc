#include "../../database/tables/relationship_table.hh"
#include "friend_remove_handler.hh"

void shiro::handler::friends::remove::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    int32_t target = in.data.read<int32_t>();

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::relationships relationships_table {};

    db(remove_from(relationships_table).where(
            relationships_table.origin == user->user_id and
            relationships_table.target == target
    ));

    auto iterator = std::find(user->friends.begin(), user->friends.end(), target);

    if (iterator == user->friends.end())
        return;

    ptrdiff_t index = std::distance(user->friends.begin(), iterator);
    user->friends.erase(user->friends.begin() + index);

    out.friend_list(user->friends);
}
