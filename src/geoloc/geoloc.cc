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
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "../logger/sentry_logger.hh"
#include "../thirdparty/json.hh"
#include "../thirdparty/loguru.hh"
#include "../utils/curler.hh"
#include "../utils/string_utils.hh"
#include "country_ids.hh"
#include "geoloc.hh"

caches::fixed_sized_cache<std::string, shiro::geoloc::location_info> shiro::geoloc::location_cache(512);

shiro::geoloc::location_info shiro::geoloc::get_location(const std::string &ip_address) {
    // For convenience
    using json = nlohmann::json;

    try {
        return location_cache.Get(ip_address);
    } catch (const std::range_error &ex) {
        // Not in cache, send request to xzq IP api and then store result in cache
    }

    const std::string &address = ip_address == "127.0.0.1" ? "self" : ip_address;
    auto [success, output] = utils::curl::get("https://ip.zxq.co/" + address);

    if (!success) {
        LOG_F(ERROR, "Unable to connect to xzq IP api: %s.", output.c_str());
        return invalid_location;
    }

    json json_result;

    try {
        json_result = json::parse(output);
    } catch (const json::parse_error &ex) {
        LOG_F(ERROR, "Unable to parse json response from xzq IP api: %s.", ex.what());
        logging::sentry::exception(ex);

        return invalid_location;
    }

    std::string country;
    std::string location;

    try {
        country = json_result["country"];
        location = json_result["loc"];
    } catch (const json::type_error &ex) {
        LOG_F(ERROR, "Received invalid response from xzq IP api: %s.", output.c_str());
        logging::sentry::exception(ex);

        return invalid_location;
    }

    if (country.empty() || location.empty()) {
        LOG_F(ERROR, "Received invalid response from xzq IP api: %s.", output.c_str());
        return invalid_location;
    }

    std::vector<std::string> result;
    boost::split(result, location, boost::is_any_of(","));

    float latitude = 0.0f;
    float longitude = 0.0f;

    try {
        latitude = boost::lexical_cast<float>(result.at(0));
        longitude = boost::lexical_cast<float>(result.at(1));
    } catch (const boost::bad_lexical_cast &ex) {
        LOG_F(WARNING, "Unable to convert latitude %s or longitude %s to float.", result.at(0).c_str(), result.at(1).c_str());
        logging::sentry::exception(ex);
    }

    location_info info(get_country_id(country), latitude, longitude);
    location_cache.Put(ip_address, info);
    return info;
}
