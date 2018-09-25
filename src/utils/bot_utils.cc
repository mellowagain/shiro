#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <utility>

#include "../bot/bot.hh"
#include "../channels/channel_manager.hh"
#include "../config/bot_file.hh"
#include "../spectating/spectator_manager.hh"
#include "bot_utils.hh"
#include "string_utils.hh"

void shiro::utils::bot::handle(shiro::io::layouts::message message, std::shared_ptr<users::user> user) {
    if (boost::algorithm::starts_with(message.content, "!")) {
        std::string removed_index = message.content.substr(1);

        std::vector<std::string> splitted;
        boost::split(splitted, removed_index, boost::is_any_of(" "));

        if (splitted.empty())
            return;

        std::string command = splitted.at(0);
        std::deque<std::string> args(splitted.begin(), splitted.end());
        args.pop_front(); // Remove command which is the first argument

        shiro::bot::handle(command, args, std::move(user), message.channel);
    }
}

void shiro::utils::bot::respond(std::string message, std::shared_ptr<shiro::users::user> user, std::string channel, bool only_user) {
    io::osu_writer buffer;
    io::layouts::message response;

    response.sender = config::bot::name;
    response.sender_id = 1;

    response.channel = channel;
    response.content = message;

    buffer.send_message(response);
    user->queue.enqueue(buffer);

    if (!boost::algorithm::starts_with(channel, "#"))
        return;

    if (only_user)
        return;

    if (channel == "#spectator") {
        std::vector<std::shared_ptr<users::user>> spectators = spectating::manager::get_spectators(user);
        std::shared_ptr<users::user> host = spectating::manager::get_host(user);

        for (const std::shared_ptr<users::user> &spectator : spectators) {
            if (spectator == user)
                continue;

            spectator->queue.enqueue(buffer);
        }

        if (host == nullptr)
            return;

        host->queue.enqueue(buffer);
        return;
    }

    auto users = channels::manager::get_users_in_channel(channel);

    for (const std::shared_ptr<users::user> &channel_user : users) {
        if (channel_user == nullptr || channel_user->user_id == user->user_id || user->user_id == 1)
            continue;

        channel_user->queue.enqueue(buffer);
    }
}
