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

#ifndef SHIRO_OSU_PACKET_HH
#define SHIRO_OSU_PACKET_HH

#include "layouts/packets.hh"
#include "osu_buffer.hh"

namespace shiro::io {

    class osu_packet {
    public:
        packet_id id;
        buffer data;

        explicit osu_packet(buffer &data);

        template<typename l>
        l unmarshal() {
            l result;
            result.unmarshal(this->data);

            return result;
        }

    };

}

#endif //SHIRO_OSU_PACKET_HH
