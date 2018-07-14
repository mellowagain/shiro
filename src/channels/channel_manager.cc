#include "../thirdparty/loguru.hh"
#include "../utils/converter.hh"
#include "channel_manager.hh"

void shiro::channels::manager::init() {
    if (!channels.empty())
        channels.clear();

    std::vector<MYSQL_ROW> rows = db_connection->query("SELECT * FROM `channels`");

    if (rows.empty())
        LOG_F(WARNING, "Received empty list of channels.");

    for (const MYSQL_ROW &row : rows) {
        uint32_t channel_id = 0;

        if (!utils::converter::str_to_uint(row[0], channel_id)) {
            LOG_F(ERROR, "Unable to convert channel id %s to uint32_t.", row[0]);
            return;
        }

        int32_t auto_join_int = 0;
        bool auto_join;

        if (!utils::converter::str_to_int(row[3], auto_join_int)) {
            LOG_F(ERROR, "Unable to convert auto join %s to uint32_t.", row[3]);
            return;
        }

        auto_join = (bool) auto_join_int;

        channels.insert(std::make_pair<io::layouts::channel, std::vector<std::shared_ptr<users::user>>>(
                io::layouts::channel(channel_id, auto_join, row[1], row[2], 0), std::vector<std::shared_ptr<users::user>>()
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
