#include "../../../utils/osu_string.hh"
#include "message.hh"

shiro::io::layouts::message::message(std::string sender, int32_t sender_id, std::string content, std::string channel)
        : sender(std::move(sender))
        , sender_id(sender_id)
        , content(std::move(content))
        , channel(std::move(channel)) {
    // Initialized in initializer list
}

std::string shiro::io::layouts::message::marshal() {
    std::string sender = utils::osu_string(this->sender);
    std::string content = utils::osu_string(this->content);
    std::string channel = utils::osu_string(this->channel);

    buffer buf;

    buf.write_int(sender.length() + content.length() + channel.length() + 4);

    buf.write_string(sender);

    buf.write_string(content);
    buf.write_string(channel);

    buf.write_int(this->sender_id);

    return buf.serialize();
}

void shiro::io::layouts::message::unmarshal(shiro::io::buffer &buffer) {
    this->sender = buffer.read_string();

    this->content = buffer.read_string();
    this->channel = buffer.read_string();

    this->sender_id = buffer.read_int();
}
