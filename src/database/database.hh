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

#ifndef SHIRO_DATABASE_HH
#define SHIRO_DATABASE_HH

#include <sqlpp11/mysql/mysql.h>
#include <sqlpp11/sqlpp11.h>

#include <memory>
#include <string>

namespace shiro {

    class database {
    private:
        std::shared_ptr<sqlpp::mysql::connection_config> config = nullptr;

        std::string address;
        uint32_t port;
        std::string db;

        std::string username;
        std::string password;

    public:
        database(const std::string &address, uint32_t port, const std::string &db, const std::string &username, const std::string &password);

        void connect();
        void setup();

        bool is_connected(bool abort = false);
        std::shared_ptr<sqlpp::mysql::connection_config> get_config();

    };

}

#endif //SHIRO_DATABASE_HH
