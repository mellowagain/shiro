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

#ifndef SHIRO_MULTIPLAYER_MATCH_HH
#define SHIRO_MULTIPLAYER_MATCH_HH

#include "../serializable.hh"

namespace shiro::io::layouts {

    class multiplayer_match : public serializable {
    public:
        std::string game_name;
        int16_t match_id;
        uint8_t multi_type;

        uint8_t multi_slot_status[16];
        int32_t multi_slot_id[16];
        uint8_t multi_slot_team[16];
        int32_t multi_slot_mods[16];

        std::string beatmap_name;
        std::string beatmap_checksum;
        int32_t beatmap_id;

        bool in_progress;
        int32_t active_mods;
        int32_t host_id;

        uint8_t game_mode;
        uint8_t multi_win_condition;
        uint8_t multi_team_type;
        uint8_t multi_special_modes;

        int32_t random_seed;
        std::string game_password;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;

    };

}

#endif //SHIRO_MULTIPLAYER_MATCH_HH
