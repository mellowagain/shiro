#include "../../../utils/osu_string.hh"
#include "user_presence.hh"

shiro::io::buffer shiro::io::layouts::user_presence::marshal() {
    std::string username = utils::osu_string(this->username);

    buffer buf;

    buf.write<int32_t>(this->user_id);
    buf.write_string(username);

    buf.write<uint8_t>(this->time_zone);
    buf.write<uint8_t>(this->country_id);

    buf.write<uint8_t>(this->permissions);

    buf.write<float>(this->longitude);
    buf.write<float>(this->latitude);

    buf.write<int32_t>(this->rank);

    return buf;
}

void shiro::io::layouts::user_presence::unmarshal(shiro::io::buffer &buffer) {
    this->user_id = buffer.read<int32_t>();
    this->username = buffer.read_string();

    this->time_zone = buffer.read<uint8_t>();
    this->country_id = buffer.read<uint8_t>();

    this->permissions = buffer.read<uint8_t>();

    this->longitude = buffer.read<float>();
    this->latitude = buffer.read<float>();

    this->rank = buffer.read<int32_t>();
}

int32_t shiro::io::layouts::user_presence::get_size() {
    return (int32_t) this->marshal().get_size();
}
