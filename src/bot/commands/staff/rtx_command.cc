#include "../../../users/user_manager.hh"
#include "../../../utils/bot_utils.hh"
#include "rtx_command.hh"

bool shiro::bot::commands::rtx(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    if (args.empty()) {
        utils::bot::respond("Usage: !rtx <target> [reason]", user, channel, true);
        return false;
    }

    std::shared_ptr<users::user> target = users::manager::get_user_by_username(args.at(0));

    if (target == nullptr || !users::manager::is_online(target)) {
        utils::bot::respond(args.at(0) + " is currently not online or does not exist.", user, channel, true);
        return false;
    }

    // No reason has been provided, set default reason
    if (args.size() == 1)
        args.emplace_back("rtx'ed by " + user->presence.username);

    args.pop_front();

    std::string reason;
    for (const std::string &arg : args) {
        reason += " " + arg;
    }

    io::osu_writer writer;
    writer.rtx(reason);

    target->queue.enqueue(writer);
    utils::bot::respond(target->presence.username + " has been successfully rtx'ed.", user, channel, true);
    return true;
}
