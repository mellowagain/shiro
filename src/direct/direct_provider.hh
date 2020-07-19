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

#ifndef SHIRO_DIRECT_PROVIDER_HH
#define SHIRO_DIRECT_PROVIDER_HH

#include <string>
#include <tuple>
#include <memory>
#include <unordered_map>

namespace shiro::direct {

    class direct_provider {
    public:
        virtual ~direct_provider() = default;

        virtual std::tuple<bool, std::string> search(std::unordered_map<std::string, std::string> parameters);
        virtual std::tuple<bool, std::string> download(int32_t beatmap_id, bool no_video);

        virtual const std::string name() const;

    };

    extern std::shared_ptr<direct_provider> provider;

    void init();

    // Checks if the current provider is working
    bool sanity_check();

}

#endif //SHIRO_DIRECT_PROVIDER_HH
