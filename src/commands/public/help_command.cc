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

#include "../../utils/bot_utils.hh"
#include "help_command.hh"

bool shiro::commands::help(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    utils::bot::respond("Commands:", user, channel, true);
    utils::bot::respond("!help - Shows list of commands", user, channel, true);
    utils::bot::respond("!pp - Shows your performance points", user, channel, true);
    utils::bot::respond("!rank - Shows your current rank on the leaderboards", user, channel, true);
    utils::bot::respond("!roll - Rolls a random number", user, channel, true);

    // TODO: Check for permissions and then display admin commands
    utils::bot::respond("Staff commands:", user, channel, true);
    utils::bot::respond("!announce - Sends a announcement to everyone or a specific user", user, channel, true);
    utils::bot::respond("!clear - Clears the chat for everyone or a specific user", user, channel, true);
    utils::bot::respond("!rtx - Send a rtx to a specific user", user, channel, true);

    return true;
}
