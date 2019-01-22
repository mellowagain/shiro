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

#include "../thirdparty/loguru.hh"
#include "../utils/curler.hh"
#include "country_ids.hh"
#include "geoloc.hh"
#include "maxmind_resolver.hh"

std::optional<shiro::geoloc::location_info> shiro::geoloc::get_location(std::string ip_address) {
    // The IP address is localhost, resolve our own geolocation
    if (ip_address == "127.0.0.1" || ip_address.empty()) {
        auto [success, output] = utils::curl::get("https://api.ipify.org/");

        if (!success) {
            LOG_F(WARNING, "Unable to resolve own local IP address: %s", output.c_str());
            return std::nullopt;
        }

        ip_address = output;
    }

    auto [country, latitude, longitude] = maxmind::locate(ip_address);
    uint8_t country_id = get_country_id(country);

    return location_info(country_id, latitude, longitude);
}
