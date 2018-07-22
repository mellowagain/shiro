#include "../../users/user_manager.hh"
#include "rtx_command.hh"

bool shiro::bot::commands::rtx(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user) {
    if (args.empty() || args.size() < 2)
        return false;

    // TODO: Check for permissions

    std::shared_ptr<users::user> target = users::manager::get_user_by_username(args.at(0));

    if (target == nullptr || !users::manager::is_online(target))
        return false;

    args.pop_front();

    std::string reason;
    for (const std::string &arg : args) {
        reason += arg;
    }

    io::osu_writer writer;
    writer.rtx(reason);

    target->queue.enqueue(writer);
    return true;
}
