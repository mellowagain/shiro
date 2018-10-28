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

#ifndef SHIRO_USER_PRESENCE_HH
#define SHIRO_USER_PRESENCE_HH

#include "../serializable.hh"

namespace shiro::io::layouts {

    class user_presence : public serializable {
    public:
        int32_t user_id = 0;
        std::string username = "";

        uint8_t time_zone = 0;
        uint8_t country_id = 0;

        uint8_t permissions = 0;

        float longitude = 0.0f;
        float latitude = 0.0f;

        int32_t rank = 0;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;

    };

}

#endif //SHIRO_USER_PRESENCE_HH
