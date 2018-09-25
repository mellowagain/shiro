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

#include "queue.hh"

void shiro::io::queue::enqueue(shiro::io::osu_writer &writer) {
    buffer buf = writer.get_buffer();
    this->packet_queue.append(buf);
}

void shiro::io::queue::enqueue(shiro::io::buffer &buffer) {
    this->packet_queue.append(buffer);
}

void shiro::io::queue::clear() {
    this->packet_queue.clear();
}

bool shiro::io::queue::is_empty() {
    return this->packet_queue.is_empty();
}

std::string shiro::io::queue::serialize() {
    std::string result = this->packet_queue.serialize();
    this->packet_queue.clear();

    return result;
}
