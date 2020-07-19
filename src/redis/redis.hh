/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2020 Marc3842h, czapek
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

#ifndef SHIRO_REDIS_HH
#define SHIRO_REDIS_HH

#include <cpp_redis/cpp_redis>
#include <cstdint>
#include <memory>
#include <string>

namespace shiro {

    class redis {
    private:
        std::shared_ptr<cpp_redis::client> client = nullptr;

        std::string address;
        uint32_t port;
        std::string password;

    public:
        redis(std::string address, uint32_t port, std::string password = "");

        void connect();
        void disconnect();
        void setup();

        bool is_connected();
        std::shared_ptr<cpp_redis::client> get();

    };

}

#endif //SHIRO_REDIS_HH
