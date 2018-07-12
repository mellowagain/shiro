#include "user_presence.hh"

std::string shiro::io::layouts::user_presence::marshal() {
    buffer buf;

    buf.write_int(this->user_id);
    buf.write_string(this->username);

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
