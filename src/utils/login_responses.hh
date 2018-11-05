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

#ifndef SHIRO_LOGIN_RESPONSES_HH
#define SHIRO_LOGIN_RESPONSES_HH

#include <cstdint>

namespace shiro::utils {

    enum class login_responses : int32_t {
        invalid_credentials = -1,
        outdated_client = -2,
        user_banned = -3,
        multiaccount_detected = -4,
        server_error = -5,
        cutting_edge_multiplayer = -6,
        account_password_rest = -7,
        verification_required = -8
    };
}

#endif // SHIRO_LOGIN_RESPONSES_HH
