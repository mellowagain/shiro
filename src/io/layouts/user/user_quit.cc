#include "user_quit.hh"

shiro::io::buffer shiro::io::layouts::user_quit::marshal() {
    io::buffer buf;

    buf.write<int32_t>(this->user_id);
    buf.write<uint8_t>(this->state);

    return buf;
}

void shiro::io::layouts::user_quit::unmarshal(shiro::io::buffer &buffer) {
    this->user_id = buffer.read<int32_t>();
    this->state = buffer.read<uint8_t>();
}

int32_t shiro::io::layouts::user_quit::get_size() {
    return 4 + 1;
}
