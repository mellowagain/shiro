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

#include <stdexcept>

#include "../config/direct_file.hh"
#include "../thirdparty/loguru.hh"
#include "providers/beatconnect.hh"
#include "providers/emulation.hh"
#include "direct_provider.hh"

std::shared_ptr<shiro::direct::direct_provider> shiro::direct::provider = nullptr;

void shiro::direct::init() {
    if (!config::direct::enabled)
        return;

    switch (config::direct::provider) {
        case 0: // Localhost Shirogane via shm
            throw std::runtime_error("Direct provider 0 (Shirogane via shared memory region) is currently not implemented.");
        case 1: // Client request emulation
            provider = std::make_shared<emulation>();
            break;
        case 2: // Beatconnect
            provider = std::make_shared<beatconnect>();
            break;
        default:
            LOG_F(ERROR, "Invalid direct mode has been provided in bancho.toml: %u.", config::direct::provider);
            break;
    }

    if (sanity_check())
        return;

    if (provider != nullptr) {
        provider.reset();
        provider = nullptr;
    }

    LOG_F(WARNING, "Sanity check has failed. Direct Beatmap downloading has been disabled.");
}

bool shiro::direct::sanity_check() {
    if (provider == nullptr)
        return false;

    auto [search_success, search_result] = provider->search({
        { "q", "hitorigoto" }
    });

    if (!search_success || search_result.empty())
        return false;

    /*auto [download_success, download_result] = provider->download(1262832, true);

    if (!download_success || download_result.empty())
        return false;*/

    LOG_F(INFO, "Direct provider has been set to %s.", provider->name().c_str());
    return true;
}

std::tuple<bool, std::string> shiro::direct::direct_provider::search(std::unordered_map<std::string, std::string> parameters) {
    return { false, "Unimplemented" };
}

std::tuple<bool, std::string> shiro::direct::direct_provider::download(int32_t beatmap_id, bool no_video) {
    return { false, "Unimplemented" };
}

const std::string shiro::direct::direct_provider::name() const {
    return "Unimplemented";
}
