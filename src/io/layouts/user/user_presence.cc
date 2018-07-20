#include "../../../utils/osu_string.hh"
#include "user_presence.hh"

std::string shiro::io::layouts::user_presence::marshal() {
    std::string username = utils::osu_string(this->username);

    buffer buf;

    buf.write_int(4 + username.length() + 1 + 1 + 1 + 4 + 4 + 4);

    buf.write_int(this->user_id);
    buf.write_string(username);

    buf.write_byte(this->time_zone);
    buf.write_byte(this->country_id);

    buf.write_byte(this->permissions);

    buf.write_float(this->longitude);
    buf.write_float(this->latitude);

    buf.write_int(this->rank);

    return buf.serialize();
}

void shiro::io::layouts::user_presence::unmarshal(shiro::io::buffer &buffer) {
    this->user_id = buffer.read_int();
    this->username = buffer.read_string();

    this->time_zone = buffer.read_byte();
    this->country_id = buffer.read_byte();

    this->permissions = buffer.read_byte();

    this->longitude = buffer.read_float();
    this->latitude = buffer.read_float();

    this->rank = buffer.read_int();
}
