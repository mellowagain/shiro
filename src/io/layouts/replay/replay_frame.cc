#include "replay_frame.hh"

shiro::io::buffer shiro::io::layouts::replay_frame::marshal() {
    buffer buf;

    buf.write<uint8_t>(this->button_state);
    buf.write<uint8_t>(this->button_type);

    buf.write<float>(this->mouse_x);
    buf.write<float>(this->mouse_y);

    buf.write<int32_t>(this->time);

    return buf;
}

void shiro::io::layouts::replay_frame::unmarshal(shiro::io::buffer &buffer) {
    this->button_state = buffer.read<uint8_t>();
    this->button_type = buffer.read<uint8_t>();

    this->mouse_x = buffer.read<float>();
    this->mouse_y = buffer.read<float>();

    this->time = buffer.read<int32_t>();
}

int32_t shiro::io::layouts::replay_frame::get_size() {
    return this->marshal().get_size();
}
