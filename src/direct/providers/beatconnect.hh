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

#ifndef SHIRO_BEATCONNECT_HH
#define SHIRO_BEATCONNECT_HH

#include "../direct_provider.hh"

namespace shiro::direct {

    class beatconnect : public direct_provider {
    public:
        std::tuple<bool, std::string> search(std::unordered_map<std::string, std::string> parameters) override;
        std::tuple<bool, std::string> download(int32_t beatmap_id, bool no_video) override;

        const std::string name() const override;

    private:
        // Translates osu!direct arguments to their Beatconnect.io equivalents
        std::tuple<std::string, std::string> sanitize_args(std::string key, std::string value, bool url_escape = false);

        static void sanitize_mode(std::string &value);
        static void sanitize_status(std::string &value);
        static void sanitize_query(std::string &value);

    };

}

#endif //SHIRO_BEATCONNECT_HH
