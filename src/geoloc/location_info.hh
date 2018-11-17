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

#ifndef SHIRO_LOCATION_INFO_HH
#define SHIRO_LOCATION_INFO_HH

#include <string>
#include <utility>

#include "country_ids.hh"

namespace shiro::geoloc {

    class location_info {
    public:
        uint8_t country;
        float latitude;
        float longitude;

        explicit location_info(uint8_t country = (uint8_t) country_id::JP, float latitude = 0.0f, float longitude = 0.0f)
                : country(country)
                , latitude(latitude)
                , longitude(longitude) {
            // Initializer in initializer list
        }

    };

    static location_info invalid_location;

}

#endif // SHIRO_LOCATION_INFO_HH
