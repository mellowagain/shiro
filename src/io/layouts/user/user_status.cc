#include "user_status.hh"

std::string shiro::io::layouts::user_status::marshal() {
    buffer buf;

    buf.write_byte(this->status);
    buf.write_string(this->status_text);

    buf.write_string(this->beatmap_checksum);

    buf.write_uint(this->current_mods);
    buf.write_byte(this->play_mode);

    buf.write_int(this->beatmap_id);

    return buf.serialize();
}

void shiro::io::layouts::user_status::unmarshal(shiro::io::buffer &buffer) {
    this->status = buffer.read_byte();
    this->status_text = buffer.read_string();

    this->beatmap_checksum = buffer.read_string();

    this->current_mods = buffer.read_uint();
    this->play_mode = buffer.read_byte();

    this->beatmap_id = buffer.read_int();
}
