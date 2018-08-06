#include "score_frame.hh"

shiro::io::buffer shiro::io::layouts::score_frame::marshal() {
    buffer buf;

    buf.write<int32_t>(this->time);
    buf.write<uint8_t>(this->id);

    buf.write<uint16_t>(this->count_300);
    buf.write<uint16_t>(this->count_100);
    buf.write<uint16_t>(this->count_50);
    buf.write<uint16_t>(this->count_geki);
    buf.write<uint16_t>(this->count_katu);
    buf.write<uint16_t>(this->count_miss);

    buf.write<int32_t>(this->total_score);
    buf.write<uint16_t>(this->max_combo);
    buf.write<uint16_t>(this->current_combo);
    buf.write<uint8_t>(this->perfect);

    buf.write<uint8_t>(this->current_hp);
    buf.write<uint8_t>(this->tag_byte);
    buf.write<uint8_t>(this->score_v2);

    if (this->score_v2) {
        buf.write<double>(this->combo_portion);
        buf.write<double>(this->bonus_portion);
    }

    return buf;
}

void shiro::io::layouts::score_frame::unmarshal(shiro::io::buffer &buffer) {
    this->time = buffer.read<int32_t>();
    this->id = buffer.read<uint8_t>();

    this->count_300 = buffer.read<uint16_t>();
    this->count_100 = buffer.read<uint16_t>();
    this->count_50 = buffer.read<uint16_t>();
    this->count_geki = buffer.read<uint16_t>();
    this->count_katu = buffer.read<uint16_t>();
    this->count_miss = buffer.read<uint16_t>();

    this->total_score = buffer.read<int32_t>();
    this->max_combo = buffer.read<uint16_t>();
    this->current_combo = buffer.read<uint16_t>();
    this->perfect = (bool) buffer.read<uint8_t>();

    this->current_hp = buffer.read<uint8_t>();
    this->tag_byte = buffer.read<uint8_t>();
    this->score_v2 = (bool) buffer.read<uint8_t>();

    if (this->score_v2) {
        this->combo_portion = buffer.read<double>();
        this->bonus_portion = buffer.read<double>();
    }
}

int32_t shiro::io::layouts::score_frame::get_size() {
    return this->marshal().get_size();
}
