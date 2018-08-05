#include "../../../utils/bot_utils.hh"
#include "help_command.hh"

bool shiro::bot::commands::help(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    utils::bot::respond("Commands:", user, channel, true);
    utils::bot::respond("!help - Shows list of commands", user, channel, true);
    utils::bot::respond("!pp - Shows your performance points", user, channel, true);
    utils::bot::respond("!rank - Shows your current rank on the leaderboards", user, channel, true);
    utils::bot::respond("!roll - Rolls a random number", user, channel, true);

    // TODO: Check for permissions and then display admin commands
    utils::bot::respond("Staff commands:", user, channel, true);
    utils::bot::respond("!announce - Sends a announcement to everyone or a user", user, channel, true);
    utils::bot::respond("!rtx - Send a rtx to a user", user, channel, true);

    return true;
}
