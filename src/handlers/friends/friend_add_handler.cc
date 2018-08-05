#include "../../database/tables/relationship_table.hh"
#include "friend_add_handler.hh"

void shiro::handler::friends::add::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    int32_t target = in.data.read<int32_t>();

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::relationships relationships_table {};

    db(insert_into(relationships_table).set(
            relationships_table.origin = user->user_id,
            relationships_table.target = target,
            relationships_table.blocked = false
    ));

    user->friends.emplace_back(target);

    out.friend_list(user->friends);
}
