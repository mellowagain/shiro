#include "../../../users/user_manager.hh"
#include "../../../utils/bot_utils.hh"
#include "announce_command.hh"

bool shiro::bot::commands::announce(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    if (args.empty()) {
        utils::bot::respond("Usage: !announce [user] <announcement>", user, channel, true);
        return false;
    }

    std::shared_ptr<users::user> target = nullptr;

    if (args.size() >= 2) {
        target = users::manager::get_user_by_username(args.at(0));

        if (target != nullptr && users::manager::is_online(target))
            args.pop_front();
    }

    std::string announcement;
    for (const std::string &arg : args) {
        announcement += " " + arg;
    }

    io::osu_writer writer;
    writer.announce(announcement);

    if (target != nullptr) {
        target->queue.enqueue(writer);
        utils::bot::respond("Successfully sent a announcement to " + target->presence.username + ".", user, channel, true);
        return true;
    }

    for (std::shared_ptr<users::user> &online_user : users::manager::online_users) {
        online_user->queue.enqueue(writer);
    }

    utils::bot::respond("Successfully sent a announcement to everyone.", user, channel, true);
    return true;
}
