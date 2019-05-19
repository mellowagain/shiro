/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2019 Marc3842h, czapek
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

#include <fstream>

#include "../thirdparty/loguru.hh"
#include "../utils/curler.hh"
#include "../utils/filesystem.hh"
#include "beatmap_helper.hh"

static fs::path dir = fs::current_path() / "maps";

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

std::optional<std::string> shiro::beatmaps::helper::get_location(int32_t beatmap_id, bool download) {
    std::string beatmap_id_str = std::to_string(beatmap_id);
    fs::path filename = dir / std::string(beatmap_id_str + ".osu");

    if (fs::exists(filename))
        return filename;

    if (!download)
        return std::nullopt;

    auto [success, output] = utils::curl::get("https://old.ppy.sh/osu/" + beatmap_id_str);

    if (!success || output.empty()) {
        LOG_F(ERROR, "Unable to connect to osu! api: %s.", output.c_str());
        return std::nullopt;
    }

    std::ofstream stream(filename);
    stream << output;

    return filename;
}
