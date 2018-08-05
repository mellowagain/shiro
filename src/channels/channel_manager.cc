#include <utility>
#include <cstring>

#include "../database/tables/channel_table.hh"
#include "../thirdparty/loguru.hh"
#include "../shiro.hh"
#include "channel_manager.hh"

std::unordered_map<shiro::io::layouts::channel, std::vector<std::shared_ptr<shiro::users::user>>> shiro::channels::manager::channels;

void shiro::channels::manager::init() {
    if (!channels.empty())
        channels.clear();

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::channels channel_table {};

    auto result = db(select(all_of(channel_table)).from(channel_table).unconditionally());
    bool empty = is_query_empty(result);

    if (empty)
        return;

    for (const auto &row : result) {
        std::string name = row.name;

        if ((int) row.id == 0)
            LOG_S(FATAL) << "Channel " << name << " uses reserved id 0, aborting!";

        if (name.find('#') == std::string::npos) {
            LOG_F(WARNING, "Channel name of channel id %i doesn't start with #, fixing (%s -> %s).", (int) row.id, name.c_str(), ("#" + name).c_str());
            name.insert(0, "#");
        }

        channels.insert(std::make_pair<io::layouts::channel, std::vector<std::shared_ptr<users::user>>>(
                io::layouts::channel(row.id, row.auto_join, name, row.description, 0),
                std::vector<std::shared_ptr<users::user>>()
        ));
    }
}

void shiro::channels::manager::write_channels(shiro::io::osu_writer &buf, std::shared_ptr<shiro::users::user> user, bool first) {
    for (auto &pair : channels) {
        shiro::io::layouts::channel channel_layout;
        channel_layout.id = pair.first.id;
        channel_layout.auto_join = pair.first.auto_join;
        channel_layout.name = pair.first.name;
        channel_layout.description = pair.first.description;
        channel_layout.user_count = pair.second.size();

        buf.channel_available(channel_layout);

        if (pair.first.auto_join && first) {
            buf.channel_join(pair.first.name);
            pair.second.emplace_back(user);
        }
    }
}

void shiro::channels::manager::join_channel(uint32_t channel_id, std::shared_ptr<shiro::users::user> user) {
    if (in_channel(channel_id, user))
        leave_channel(channel_id, std::move(user));

    for (auto &pair : channels) {
        if (pair.first.id == channel_id) {
            pair.second.emplace_back(user);
            break;
        }
    }
}

void shiro::channels::manager::leave_channel(uint32_t channel_id, std::shared_ptr<shiro::users::user> user) {
    if (!in_channel(channel_id, user))
        return;

    for (auto &pair : channels) {
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

std::vector<std::shared_ptr<shiro::users::user>> shiro::channels::manager::get_users_in_channel(const std::string &channel_name) {
    for (const auto &pair : channels) {
        if (pair.first.name == channel_name)
            return pair.second;
    }

    return {};
}

uint32_t shiro::channels::manager::get_channel_id(const std::string &channel_name) {
    for (const auto &pair : channels) {
        if (pair.first.name == channel_name)
            return pair.first.id;
    }

    return 0;
}
