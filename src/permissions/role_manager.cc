/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2019 Marc3842h, czapek
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

#include "../config/bancho_file.hh"
#include "../database/tables/roles_table.hh"
#include "../utils/crypto.hh"
#include "../utils/osu_permissions.hh"
#include "../shiro.hh"
#include "role_manager.hh"

std::vector<shiro::permissions::role> shiro::roles::manager::roles;

void shiro::roles::manager::init() {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::roles roles_table {};

    auto result = db(select(all_of(roles_table)).from(roles_table).unconditionally());

    for (const auto &row : result) {
        permissions::role role(row.id, row.name, row.permissions, row.color);
        roles.emplace_back(role);
    }
}

bool shiro::roles::manager::has_permission(std::shared_ptr<shiro::users::user> user, shiro::permissions::perms permissions) {
    return has_permission(std::move(user), (uint64_t) permissions);
}

bool shiro::roles::manager::has_permission(std::shared_ptr<users::user> user, uint64_t permissions) {
    if (user == nullptr)
        return false;

    for (const permissions::role &role : roles) {
        if (!(user->roles & role.id))
            continue;

        if (!(role.permissions & permissions))
            continue;

        return true;
    }

    return false;
}

uint8_t shiro::roles::manager::get_chat_color(uint32_t roles) {
    if (roles == 0xDEADCAFE)
        return (uint8_t) utils::osu_permissions::friend_;

    uint8_t result = (uint8_t) utils::osu_permissions::normal;

    if (config::bancho::default_supporter)
        result |= (uint8_t) utils::osu_permissions::supporter;

    uint32_t highest_role = utils::crypto::get_highest_bit(roles);

    if (highest_role == 0)
        return result;

    for (const permissions::role &role : manager::roles) {
        if (role.id != highest_role)
            continue;

        result |= role.color;
        break;
    }

    return result;
}
