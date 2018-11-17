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
#include <curl/curl.h>

#include "../thirdparty/json.hh"
#include "../thirdparty/loguru.hh"
#include "../utils/string_utils.hh"
#include "../utils/curler.hh"
#include "country_ids.hh"
#include "geoloc.hh"

caches::fixed_sized_cache<std::string, shiro::geoloc::location_info> shiro::geoloc::location_cache(512);

shiro::geoloc::location_info shiro::geoloc::get_location(const std::string &ip_address) {
    // For convenience
    using json = nlohmann::json;

    try {
        return location_cache.Get(ip_address);
    } catch (const std::range_error &ex) {
        // Not in cache, send request to Gitea API and then store result in cache
    }

    shiro::utils::curler curl;
    CURLcode status_code;

    std::string output;

    if (curl.valid()) {
        char buffer[512];
        std::snprintf(buffer, sizeof(buffer), "https://ip.zxq.co/%s", ip_address.c_str());

        curl.set_callback(callback);
        curl.set_output(&output);
        curl.set_user_agent("shiro (https://github.com/Marc3842h/shiro)");

        status_code = curl.perform(buffer);

        if (status_code != CURLE_OK) {
            LOG_F(ERROR, "Received invalid response from Gitea API: %s.", curl_easy_strerror(status_code));
            return invalid_location;
        }
    }

    if (!boost::algorithm::starts_with(output, "{")) {
        LOG_F(ERROR, "Received invalid response from Gitea API: %s.", output.c_str());
        return invalid_location;
    }

    auto json_result = json::parse(output);

    std::string country;
    std::string location;

    try {
        country = json_result["country"];
        location = json_result["loc"];
    } catch (const json::type_error &ex) {
        LOG_F(ERROR, "Received invalid response from Gitea API: %s.", output.c_str());
        return invalid_location;
    }

    if (country.empty() || location.empty()) {
        LOG_F(ERROR, "Received invalid response from Gitea API: %s.", output.c_str());
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
    }

    location_info info(get_country_id(country), latitude, longitude);
    location_cache.Put(ip_address, info);
    return info;
}

size_t shiro::geoloc::callback(void *raw_data, size_t size, size_t memory, std::string *ptr) {
    size_t new_length = size * memory;
    size_t old_length = ptr->size();

    try {
        ptr->resize(old_length + new_length);
    } catch (const std::bad_alloc &ex) {
        LOG_F(ERROR, "Unable to allocate new memory for callback of geo location service: %s.", ex.what());
        return 0;
    }

    std::copy((char*) raw_data, (char*) raw_data + new_length, ptr->begin() + old_length);
    return size * memory;
}
