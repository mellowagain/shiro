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

#include "../../utils/bot_utils.hh"
#include "mp_command.hh"
#include "../../users/user_manager.hh"
#include "../../multiplayer/match_manager.hh"
#include "../../permissions/role_manager.hh"
#include "../../utils/slot_status.hh"

bool shiro::commands::mp(std::deque<std::string> &args, std::shared_ptr<users::user> user, std::string channel) {
    if (args.size() == 0) {
    ok:
        utils::bot::respond("Multiplayer commands (only for host):", user, channel, true);
        utils::bot::respond("!mp start - Starts the game", user, channel, true);
        utils::bot::respond("!mp abort - Aborts the live game", user, channel, true);
        utils::bot::respond("!mp ready - Marks everyone as ready", user, channel, true);
        utils::bot::respond("!mp host <username> - Sets a new room host", user, channel, true);

        return true;
    }

    std::optional<io::layouts::multiplayer_match> optional = multiplayer::match_manager::get_match(user);

    if (!optional.has_value()) {
        utils::bot::respond("You need to be in a room to use this command", user, channel, true);
        return true;
    }

    io::layouts::multiplayer_match match = *optional;

    if (match.host_id != user->user_id && !roles::manager::has_permission(user, permissions::perms::channel_console)) {
        utils::bot::respond("You need to be host to use this command", user, channel, true);
        return true;
    }

    std::string command = args.at(0);

    if (args.size() == 1) {
        bool start = command == "start";
        bool abort = command == "abort";

        if (start || abort) {
            io::osu_writer writer;

            if (start) {
                writer.match_start(match, false);
            } else if (abort) {
                writer.match_abort();
            }

            match.in_progress = start;

            for (size_t i = 0; i < match.multi_slot_id.size(); i++) {
                if (match.multi_slot_id.at(i) == -1)
                    continue;

                match.multi_slot_status.at(i) = start
                        ? (uint8_t) utils::slot_status::playing
                        : (uint8_t) utils::slot_status::not_ready;

                std::shared_ptr<users::user> lobby_user = users::manager::get_user_by_id(match.multi_slot_id.at(i));

                if (lobby_user == nullptr)
                    continue;

                lobby_user->queue.enqueue(writer);
            }

            std::string msg = "Match has been forcefully ";

            match.send_update(true);
            utils::bot::respond(msg + (start ? "started" : "aborted"), user, channel, true);
            return true;
        } else if (command == "ready") {
            for (size_t i = 0; i < match.multi_slot_id.size(); i++) {
                if (match.multi_slot_id.at(i) == -1)
                    continue;

                match.multi_slot_status.at(i) = (uint8_t) utils::slot_status::ready;
            }

            match.send_update(false);
            utils::bot::respond("Everybody is now ready", user, channel, true);
            return true;
        }
    }

    // Must be !mp host
    if (args.size() == 2 && command == "host") {
        std::shared_ptr<users::user> new_host = users::manager::get_user_by_username(args.at(1));

        if (new_host == nullptr) {
            utils::bot::respond("New host is not online", user, channel, true);
            return true;
        }

        match.host_id = new_host->user_id;
        match.send_update(true);

        utils::bot::respond(new_host->presence.username + " is now the new host", user, channel, true);
        return true;
    }

    goto ok;
}
