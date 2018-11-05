/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018 Marc3842h, czapek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "../../database/tables/relationship_table.hh"
#include "friend_remove_handler.hh"

void shiro::handler::friends::remove::handle(shiro::io::osu_packet &in, shiro::io::osu_writer &out, std::shared_ptr<shiro::users::user> user) {
    int32_t target = in.data.read<int32_t>();

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::relationships relationships_table{};

    db(remove_from(relationships_table).where(relationships_table.origin == user->user_id and relationships_table.target == target));

    auto iterator = std::find(user->friends.begin(), user->friends.end(), target);

    if (iterator == user->friends.end())
        return;

    ptrdiff_t index = std::distance(user->friends.begin(), iterator);
    user->friends.erase(user->friends.begin() + index);

    out.friend_list(user->friends);
}
