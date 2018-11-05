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

#ifndef SHIRO_USER_PUNISHMENTS_HH
#define SHIRO_USER_PUNISHMENTS_HH

#include <memory>
#include <string>
#include <tuple>

#include "../config/bot_file.hh"

namespace shiro::users::punishments {

    void init();

    void kick(int32_t user_id, const std::string &reason = "You have been kicked. Please login again.");
    void silence(int32_t user_id, uint32_t duration, const std::string &reason = "Silenced (" + config::bot::name + " protection)");
    void restrict(int32_t user_id, const std::string &reason = "Your account has been flagged.");
    void ban(int32_t user_id, const std::string &reason = "Your account has been convicted.");

    bool is_silenced(int32_t user_id);
    bool is_restricted(int32_t user_id);
    bool is_banned(int32_t user_id);

    bool can_chat(int32_t user_id);
    bool has_scores(int32_t user_id);

    std::tuple<int32_t, uint32_t> get_silence_time(int32_t user_id);
}

#endif //SHIRO_USER_PUNISHMENTS_HH
