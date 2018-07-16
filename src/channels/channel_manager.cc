#include <utility>

#include "../thirdparty/loguru.hh"
#include "channel_manager.hh"

void shiro::channels::manager::init() {
    if (!channels.empty())
        channels.clear();

    using channel_struct = std::tuple<uint32_t, std::string, std::string, bool>;
    auto result = db_connection->query<channel_struct>("SELECT * FROM `channels`");

    if (result.empty())
        return;

    for (const channel_struct &channel_struct : result) {
        uint32_t channel_id = std::get<0>(channel_struct);
        std::string name = std::get<1>(channel_struct);
        const std::string &description = std::get<2>(channel_struct);
        bool auto_join = std::get<3>(channel_struct);

        if (name.find('#') == std::string::npos) {
            LOG_F(WARNING, "Channel name of channel id %i doesn't start with #, fixing (%s -> %s).", channel_id, name.c_str(), ("#" + name).c_str());
            name.insert(0, "#");
        }

        channels.insert(std::make_pair<io::layouts::channel, std::vector<std::shared_ptr<users::user>>>(
                io::layouts::channel(channel_id, auto_join, name, description, 0),
                std::vector<std::shared_ptr<users::user>>()
        ));
    }
}

void shiro::channels::manager::write_channels(shiro::io::osu_writer &buf) {
    for (const auto &pair : channels) {
        buf.channel_available(pair.first);

        if (pair.first.auto_join)
            buf.channel_join(pair.first.name);
    }
}

void shiro::channels::manager::join_channel(uint32_t channel_id, std::shared_ptr<shiro::users::user> user) {
    if (in_channel(channel_id, user))
        leave_channel(channel_id, std::move(user));

    for (auto pair : channels) {
        if (pair.first.id == channel_id) {
            pair.second.emplace_back(user);
            break;
        }
    }
}

void shiro::channels::manager::leave_channel(uint32_t channel_id, std::shared_ptr<shiro::users::user> user) {
    if (!in_channel(channel_id, user))
        return;

    for (auto pair : channels) {
        if (pair.first.id == channel_id) {
            auto iterator = std::find(pair.second.begin(), pair.second.end(), user);

            if (iterator == pair.second.end())
                return;

            ptrdiff_t index = std::distance(pair.second.begin(), iterator);
            pair.second.erase(pair.second.begin() + index);
            break;
        }
    }
}

bool shiro::channels::manager::in_channel(uint32_t channel_id, const std::shared_ptr<shiro::users::user> &user) {
    for (const auto &pair : channels) {
        if (pair.first.id == channel_id)
            return std::find(pair.second.begin(), pair.second.end(), user) != pair.second.end();
    }

    return false;
}
