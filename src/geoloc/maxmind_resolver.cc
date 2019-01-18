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

#include <cstring>
#include <maxminddb.h>

#include "../thirdparty/loguru.hh"
#include "../utils/curler.hh"
#include "maxmind_resolver.hh"

static MMDB_s mmdb;

void shiro::geoloc::maxmind::init() {
    int code = MMDB_open("geolite2_city.mmdb", MMDB_MODE_MMAP, &mmdb);

    if (code == MMDB_SUCCESS)
        return;

    LOG_F(FATAL, "Unable to open maxmind db database: %s", MMDB_strerror(code));
}

void shiro::geoloc::maxmind::destroy() {
    MMDB_close(&mmdb);
}

std::tuple<std::string, int, int> shiro::geoloc::maxmind::locate(std::string ip_address) {
    int gai_error = 0;
    int mmdb_error = 0;

    MMDB_lookup_result_s result = MMDB_lookup_string(&mmdb, ip_address.c_str(), &gai_error, &mmdb_error);

    if (gai_error != 0) {
        LOG_F(WARNING, "Unable to get address info: %s", std::strerror(gai_error));
        return { "XX", 0, 0 };
    }

    if (mmdb_error != MMDB_SUCCESS) {
        LOG_F(WARNING, "Unable to resolve ip address in maxmind database: %s", MMDB_strerror(mmdb_error));
        return { "XX", 0, 0 };
    }

    if (!result.found_entry) {
        LOG_F(WARNING, "IP address does not exist in maxmind database: %s", ip_address.c_str());
        return { "XX", 0, 0 };
    }

    MMDB_entry_data_s country_entry_data; // utf-8 string
    MMDB_entry_data_s latitude_entry_data; // double
    MMDB_entry_data_s longitude_entry_data; // double

    int country_status = MMDB_get_value(&result.entry, &country_entry_data, "country", "iso_code", nullptr);
    int latitude_status = MMDB_get_value(&result.entry, &latitude_entry_data, "location", "latitude", nullptr);
    int longitude_status = MMDB_get_value(&result.entry, &longitude_entry_data, "location", "longitude", nullptr);

    if (country_status != MMDB_SUCCESS || !country_entry_data.has_data) {
        LOG_F(WARNING, "Unable to resolve country for address %s", ip_address.c_str());
        return { "XX", 0, 0 };
    }

    std::string country = country_entry_data.utf8_string;
    country.resize(2, 'X');

    if (latitude_status != MMDB_SUCCESS || !latitude_entry_data.has_data) {
        LOG_F(WARNING, "Unable to resolve latitude for address %s", ip_address.c_str());
        return { country, 0, 0 };
    }

    if (longitude_status != MMDB_SUCCESS || !longitude_entry_data.has_data) {
        LOG_F(WARNING, "Unable to resolve longitude for address %s", ip_address.c_str());
        return { country, latitude_entry_data.double_value, 0 };
    }

    return { country, latitude_entry_data.double_value, longitude_entry_data.double_value };
}
