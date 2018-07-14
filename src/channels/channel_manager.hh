#ifndef SHIRO_CHANNEL_MANAGER_HH
#define SHIRO_CHANNEL_MANAGER_HH

#include <unordered_map>

#include "../io/layouts/channel/channel.hh"
#include "../io/osu_writer.hh"
#include "../users/user.hh"

namespace shiro::channels::manager {

    static std::unordered_map<io::layouts::channel, std::vector<std::shared_ptr<users::user>>> channels;

    void init();

    void write_channels(io::osu_writer &buf);

    void write_auto_join_channels(io::osu_writer &buf);

}

#endif //SHIRO_CHANNEL_MANAGER_HH
