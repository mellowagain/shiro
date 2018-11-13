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

#include "../utils/filesystem.hh"

#include <curl/curl.h>
#include <fstream>

#include "../thirdparty/loguru.hh"
#include "beatmap_helper.hh"

static std::string dir = fs::current_path().u8string() + shiro::utils::filesystem::preferred_separator + "maps";

void shiro::beatmaps::helper::init() {
    if (!fs::exists(dir))
        fs::create_directories(dir);
}

int32_t shiro::beatmaps::helper::fix_beatmap_status(int32_t status_code) {
    if (status_code == (int32_t) status::needs_update)
        return (int32_t) status::ranked;

    if (status_code == (int32_t) status::qualified)
        return (int32_t) status::loved;

    return status_code;
}

bool shiro::beatmaps::helper::has_leaderboard(int32_t status_code) {
    return status_code == (int32_t) status::ranked ||
           status_code == (int32_t) status::loved ||
           status_code == (int32_t) status::qualified ||
           status_code == (int32_t) status::approved;
}

bool shiro::beatmaps::helper::awards_pp(int32_t status_code) {
    return status_code == (int32_t) status::ranked;
}

size_t shiro::beatmaps::helper::callback(void *raw_data, size_t size, size_t memory, std::string *ptr) {
    size_t new_length = size * memory;
    size_t old_length = ptr->size();

    try {
        ptr->resize(old_length + new_length);
    } catch (const std::bad_alloc &ex) {
        LOG_F(ERROR, "Unable to allocate new memory for callback of beatmap helper: %s.", ex.what());
        return 0;
    }

    std::copy((char*) raw_data, (char*) raw_data + new_length, ptr->begin() + old_length);
    return size * memory;
}

FILE *shiro::beatmaps::helper::download(int32_t beatmap_id) {
    std::string filename = dir + utils::filesystem::preferred_separator + std::to_string(beatmap_id) + ".osu";

    if (fs::exists(filename))
        return std::fopen(filename.c_str(), "rb");

    CURL *curl = curl_easy_init();
    CURLcode status_code;

    std::string output;

    if (curl != nullptr) {
        char buffer[512];
        std::snprintf(buffer, sizeof(buffer), "https://old.ppy.sh/osu/%i", beatmap_id);

        curl_easy_setopt(curl, CURLOPT_URL, buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64; rv:61.0) Gecko/20100101 Firefox/61.0"); // shiro (https://github.com/Marc3842h/shiro)

        status_code = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (status_code != CURLE_OK) {
            LOG_F(ERROR, "Received invalid response from Bancho API: %s.", curl_easy_strerror(status_code));
            return nullptr;
        }
    }

    FILE *map_file = std::fopen(filename.c_str(), "wb");

    std::fwrite(output.c_str(), sizeof(char), output.length(), map_file);
    std::fclose(map_file);

    return std::fopen(filename.c_str(), "rb");
}
