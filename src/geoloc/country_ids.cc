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

#include <algorithm>

#include "../thirdparty/loguru.hh"
#include "country_ids.hh"

uint8_t shiro::geoloc::get_country_id(const std::string &country_code) {
    better_enums::optional<country_id> id = country_id::_from_string_nothrow(country_code.c_str());

    if (!id) {
        LOG_F(WARNING, "Unable to find country id for country %s.", country_code.c_str());
        return (uint8_t) country_id::JP; // Default to Japan because we're weebs ¯\_(ツ)_/¯
    }

    return id->_to_integral();
}

std::string shiro::geoloc::get_country_name(uint8_t country) {
    better_enums::optional<country_id> id = country_id::_from_integral_nothrow(country);

    if (!id) {
        LOG_F(WARNING, "Unable to find country name for country id: %hhu", country);
        return "JP"; // Default to Japan because we're weebs ¯\_(ツ)_/¯
    }

    return id->_to_string();
}
