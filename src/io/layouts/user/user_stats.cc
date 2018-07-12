#include "user_stats.hh"

std::string shiro::io::layouts::user_stats::marshal() {
    buffer buf;

    buf.write_int(this->user_id);

    buf.write_byte(this->status);
    buf.write_string(this->status_text);

    buf.write_string(this->beatmap_checksum);

    buf.write_uint(this->current_mods);
    buf.write_byte(this->play_mode);

    buf.write_int(this->beatmap_id);

    buf.write_ulong(this->ranked_score);

    buf.write_float(this->accuracy);
    buf.write_int(this->play_count);
    buf.write_ulong(this->total_score);

    buf.write_int(this->rank);
    buf.write_short(this->pp);

    return buf.serialize();
}

void shiro::io::layouts::user_stats::unmarshal(shiro::io::buffer &buffer) {
    this->user_id = buffer.read_int();

    this->status = buffer.read_byte();
    this->status_text = buffer.read_string();

    this->beatmap_checksum = buffer.read_string();

    this->current_mods = buffer.read_uint();
    this->play_mode = buffer.read_byte();

    this->beatmap_id = buffer.read_int();

    this->ranked_score = buffer.read_ulong();

    this->accuracy = buffer.read_float();
    this->play_count = buffer.read_int();
    this->total_score = buffer.read_ulong();

    this->rank = buffer.read_int();
    this->pp = buffer.read_short();
}
