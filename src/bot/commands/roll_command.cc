#include <random>

#include "../../utils/bot_utils.hh"
#include "roll_command.hh"

static std::random_device random_device;
static std::mt19937 engine(random_device());

bool shiro::bot::commands::roll(std::deque<std::string> &args, std::shared_ptr<shiro::users::user> user, std::string channel) {
    uint32_t max = 100;

    if (!args.empty()) {
        try {
            max = boost::lexical_cast<uint32_t>(args.at(0));
        } catch (const boost::bad_lexical_cast&) {
            // User wrote some non-sense, let's ignore and roll with default 100
        }
    }

    std::uniform_int_distribution<> distribution(0, max);
    int roll = distribution(engine);
    std::string extension = roll != 1 ? "s" : "";

    utils::bot::respond(user->presence.username + " rolls " + std::to_string(roll) + " point" + extension + ".", user, channel);
    return true;
}
