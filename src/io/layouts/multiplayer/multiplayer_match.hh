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

#ifndef SHIRO_MULTIPLAYER_MATCH_HH
#define SHIRO_MULTIPLAYER_MATCH_HH

#include <array>

#include "../../../utils/mods.hh"
#include "../serializable.hh"

namespace shiro::io::layouts {

    class multiplayer_match : public serializable {
    public:
        virtual ~multiplayer_match() = default;

        std::string game_name = "";
        uint16_t match_id = 0;
        uint8_t multi_type = 0;

        std::array<uint8_t, 16> multi_slot_status {};
        std::array<int32_t, 16> multi_slot_id {};
        std::array<uint8_t, 16> multi_slot_team {};
        std::array<int32_t, 16> multi_slot_mods {};

        std::array<bool, 16> loaded_players {};
        std::array<bool, 16> skipped_players {};
        std::array<bool, 16> finished_players {};

        std::string beatmap_name = "";
        std::string beatmap_checksum = "";
        int32_t beatmap_id = 0;

        bool in_progress = false;
        int32_t active_mods = (int32_t) utils::mods::none;
        int32_t host_id = 0;

        uint8_t game_mode = 0;
        uint8_t multi_win_condition = 0;
        uint8_t multi_team_type = 0;
        uint8_t multi_special_modes = 0; // 1 = Free Mod

        int32_t random_seed = 0;
        std::string game_password = "";

        void send_update(bool global);

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;

    };

}

#endif //SHIRO_MULTIPLAYER_MATCH_HH
