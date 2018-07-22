#include "user_status.hh"

shiro::io::buffer shiro::io::layouts::user_status::marshal() {
    buffer buf;

    return buf;
}

void shiro::io::layouts::user_status::unmarshal(shiro::io::buffer &data) {
    this->activity = data.read<uint8_t>();
    this->activity_desc = data.read_string();

    this->beatmap_checksum = data.read_string();

    this->current_mods = data.read<uint32_t>();
    this->play_mode = data.read<uint8_t>();

    this->beatmap_id = data.read<int32_t>();
}

int32_t shiro::io::layouts::user_status::get_size() {
    return (int32_t) this->marshal().get_size();
}
