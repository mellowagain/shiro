#include "../thirdparty/loguru.hh"
#include "../utils/converter.hh"
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
        const std::string &name = std::get<1>(channel_struct);
        const std::string &description = std::get<2>(channel_struct);
        bool auto_join = std::get<3>(channel_struct);

        channels.insert(std::make_pair<io::layouts::channel, std::vector<std::shared_ptr<users::user>>>(
                io::layouts::channel(channel_id, auto_join, name, description, 0),
                std::vector<std::shared_ptr<users::user>>()
        ));
    }
}

void shiro::channels::manager::write_channels(shiro::io::osu_writer &buf) {
    for (const auto &pair : channels) {
        buf.channel_available(pair.first);
    }
}

void shiro::channels::manager::write_auto_join_channels(shiro::io::osu_writer &buf) {
    for (const auto &pair : channels) {
        if (pair.first.auto_join)
            buf.channel_join(pair.first.name);
    }
}
