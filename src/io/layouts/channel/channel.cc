#include <utility>

#include "../../../utils/osu_string.hh"
#include "channel.hh"

shiro::io::layouts::channel::channel(uint32_t id, bool auto_join, std::string name, std::string description, int32_t user_count)
        : id(id)
        , auto_join(auto_join)
        , name(std::move(name))
        , description(std::move(description))
        , user_count(user_count) {
    // Initialized in initializer list
}

std::string shiro::io::layouts::channel::marshal() {
    std::string name = utils::osu_string(this->name);
    std::string description = utils::osu_string(this->description);

    buffer buf;

    buf.write_int(name.length() + description.length() + 4);
    buf.write_string(name);
    buf.write_string(description);
    buf.write_int(this->user_count);

    return buf.serialize();
}

void shiro::io::layouts::channel::unmarshal(shiro::io::buffer &buffer) {
    this->name = buffer.read_string();
    this->description = buffer.read_string();
    this->user_count = buffer.read_int();
}

bool shiro::io::layouts::channel::operator==(const shiro::io::layouts::channel &other_channel) const {
    return (this->name == other_channel.name && this->description == other_channel.description);
}

bool shiro::io::layouts::channel::operator!=(const shiro::io::layouts::channel &other_channel) const {
    return !operator==(other_channel);
}

size_t std::hash<shiro::io::layouts::channel>::operator()(const shiro::io::layouts::channel &channel) const {
    size_t id_hash = std::hash<uint32_t>()(channel.id);
    size_t name_hash = std::hash<std::string>()(channel.name);
    size_t description_hash = std::hash<std::string>()(channel.description);

    return ((id_hash ^ (name_hash << 1)) >> 1) ^ (description_hash << 1);
}
