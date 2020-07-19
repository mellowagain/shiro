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

#include <boost/lexical_cast.hpp>

#include "../../logger/sentry_logger.hh"
#include "../../thirdparty/loguru.hh"
#include "../../utils/curler.hh"
#include "../../utils/play_mode.hh"
#include "beatconnect.hh"

std::tuple<bool, std::string> shiro::direct::beatconnect::search(std::unordered_map<std::string, std::string> parameters) {
    // Remove username from the request so the requesting user stays anonymous
    if (parameters.find("u") != parameters.end())
        parameters.erase("u");

    // Remove password hash from the request so no credentials are leaked
    if (parameters.find("h") != parameters.end())
        parameters.erase("h");

    std::string url = "https://beatconnect.io/api/search/?";

    for (const auto &[key, value] : parameters) {
        auto [sane_key, sane_value] = sanitize_args(key, value, true);

        url.append(sane_key).append("=").append(sane_value).append("&");
    }

    // Remove the last char (which will be a &)
    url = url.substr(0, url.length() - 1);

    auto [success, output] = utils::curl::get_direct(url);

    if (!success)
        return { false, output };

    json json_result;

    try {
        json_result = json::parse(output);
    } catch (const json::parse_error &ex) {
        LOG_F(ERROR, "Unable to parse json response from Beatconnect: %s.", ex.what());
        logging::sentry::exception(ex);

        return { false, ex.what() };
    }

    std::stringstream out;
    out << (int32_t) json_result["max_page"] << std::endl;

    for (auto &map_json : json_result["beatmaps"]) {
        std::string beatmap_id = std::to_string((int32_t) map_json["id"]);
        std::string last_updated = "-";

        if (map_json["last_updated"].is_string())
            last_updated = map_json["last_updated"];

        out << beatmap_id << ".osz" << "|"; // Filename
        out << (std::string) map_json["artist"] << "|"; // Artist
        out << (std::string) map_json["title"] << "|"; // Song
        out << (std::string) map_json["creator"] << "|"; // Mapper
        out << 1 << "|"; // ?
        out << 0.0 << "|"; // Average Rating
        out << last_updated << "|"; // Last updated
        out << beatmap_id << "|"; // Beatmap id
        out << beatmap_id << "|"; // Beatmap id?
        out << 0 << "|"; // ?
        out << 0 << "|"; // ?
        out << 0 << "|"; // ?
        out << "|"; // Start of difficulties

        std::stringstream difficulties;
        difficulties << std::setprecision(2);

        for (auto &diff_json : map_json["beatmaps"]) {
            difficulties << (std::string) diff_json["version"] << " (";
            difficulties << (float) diff_json["difficulty"] << "★~";
            difficulties << "AR" << (float) diff_json["ar"] << "~";
            difficulties << "CS" << (float) diff_json["cs"] << "~";
            difficulties << "HP" << (float) diff_json["drain"] << "~";

            int32_t total_length = diff_json["total_length"];
            int32_t minutes = total_length / 60;
            int32_t seconds = total_length % 60;

            if (minutes > 0)
                difficulties << minutes << "m";

            difficulties << seconds << "s)" << "@";
            difficulties << (int32_t) diff_json["mode_int"] << ",";
        }

        std::string all_difficulties = difficulties.str();
        all_difficulties.pop_back();

        out << all_difficulties;
        out << "|"; // End of difficulties
        out << "\n"; // std::endl flushes additionally which is not something we want
    }

    return { true, out.str() };
}

std::tuple<bool, std::string> shiro::direct::beatconnect::download(int32_t beatmap_id, bool no_video) {
    std::string id = std::to_string(beatmap_id);
    auto [success, output] = utils::curl::get_direct("https://beatconnect.io/api/beatmap/" + id + "/");

    if (!success)
        return { success, output };

    json json_result;

    try {
        json_result = json::parse(output);
    } catch (const json::parse_error &ex) {
        logging::sentry::exception(ex);
        return { false, ex.what() };
    }

    std::string unique_id = json_result["unique_id"];
    std::string url = "https://beatconnect.io/b/" + id + "/" + unique_id + "/";

    if (no_video)
        url += "?novideo=1";

    return utils::curl::get_direct(url);
}

const std::string shiro::direct::beatconnect::name() const {
    return "Beatconnect";
}

std::tuple<std::string, std::string> shiro::direct::beatconnect::sanitize_args(std::string key, std::string value, bool url_escape) {
    if (key == "m")
        sanitize_mode(value);

    if (key == "r") {
        key = "s";
        sanitize_status(value);
    }

    if (key == "q")
        sanitize_query(value);

    // Escape parameters to be safely used in urls
    if (url_escape) {
        key = utils::curl::escape_url(key);
        value = utils::curl::escape_url(value);
    }

    return { key, value };
}

void shiro::direct::beatconnect::sanitize_mode(std::string &value) {
    int32_t mode = 0;

    try {
        mode = boost::lexical_cast<int32_t>(value);
    } catch (const boost::bad_lexical_cast &ex) {
        logging::sentry::exception(ex);

        LOG_F(WARNING, "Unable to cast %s to int32_t: %s", value.c_str(), ex.what());
        return;
    }

    // Check if we're within enum range to avoid undefined behaviour
    if (mode < 0 || mode > 3) {
        value = "all";
        return;
    }

    value = utils::play_mode_to_string((utils::play_mode) mode, false);
}

void shiro::direct::beatconnect::sanitize_status(std::string &value) {
    static std::unordered_map<std::string, std::string> ranked_status_mapping = {
            { "0", "ranked" },
            { "1", "ranked" },
            { "2", "loved" },
            { "3", "qualified" },
            { "4", "unranked" },
            { "5", "unranked" }
    };

    if (ranked_status_mapping.find(value) != ranked_status_mapping.end()) {
        value = ranked_status_mapping.at(value);
        return;
    }

    value = "all";
}

void shiro::direct::beatconnect::sanitize_query(std::string &value) {
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);

    boost::replace_all(value, "newest", "");
    boost::replace_all(value, "most played", "");
    boost::replace_all(value, "top rated", "");
}
