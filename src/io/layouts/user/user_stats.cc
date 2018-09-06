#include <cmath>

#include "../../../thirdparty/loguru.hh"
#include "../../../scores/score.hh"
#include "../../../scores/score_helper.hh"
#include "../../../utils/osu_string.hh"
#include "../../../utils/play_mode.hh"
#include "user_stats.hh"

void shiro::io::layouts::user_stats::recalculate_accuracy() {
    std::vector<scores::score> scores = scores::helper::fetch_top100_user((utils::play_mode) this->play_mode, this->user_id);
    float accuracy = 0.0f;

    for (size_t i = 0; i < scores.size(); i++) {
        scores::score score = scores.at(i);

        accuracy += (score.accuracy * std::pow(0.95, i));
    }

    this->accuracy = accuracy / scores.size();
}

void shiro::io::layouts::user_stats::recalculate_pp() {
    std::vector<scores::score> scores = scores::helper::fetch_top100_user((utils::play_mode) this->play_mode, this->user_id);
    float pp = 0; // Here it is a float to keep decimal points, round it when setting final pp value

    for (size_t i = 0; i < scores.size(); i++) {
        scores::score score = scores.at(i);

        pp += (score.pp * std::pow(0.95, i));
    }

    this->pp = static_cast<int16_t>(pp);
}

shiro::io::buffer shiro::io::layouts::user_stats::marshal() {
    std::string status_text = utils::osu_string(this->activity_desc);
    std::string beatmap_checksum = utils::osu_string(this->beatmap_checksum);

    buffer buf;

    buf.write<int32_t>(this->user_id);

    buf.write<uint8_t>(this->activity);
    buf.write_string(status_text);

    buf.write_string(beatmap_checksum);

    buf.write<uint32_t>(this->current_mods);
    buf.write<uint8_t>(this->play_mode);

    buf.write<int32_t>(this->beatmap_id);

    buf.write<uint64_t>(this->ranked_score);

    buf.write<float>(this->accuracy / 100);
    buf.write<int32_t>(this->play_count);
    buf.write<uint64_t>(this->total_score);

    buf.write<int32_t>(this->rank);
    buf.write<int16_t>(this->pp);

    return buf;
}

void shiro::io::layouts::user_stats::unmarshal(shiro::io::buffer &buffer) {
    this->user_id = buffer.read<int32_t>();

    this->activity = buffer.read<uint8_t>();
    this->activity_desc = buffer.read_string();

    this->beatmap_checksum = buffer.read_string();

    this->current_mods = buffer.read<uint32_t>();
    this->play_mode = buffer.read<uint8_t>();

    this->beatmap_id = buffer.read<int32_t>();

    this->ranked_score = buffer.read<uint64_t>();

    this->accuracy = buffer.read<float>() * 100;
    this->play_count = buffer.read<int32_t>();
    this->total_score = buffer.read<uint64_t>();

    this->rank = buffer.read<int32_t>();
    this->pp = buffer.read<int16_t>();
}

int32_t shiro::io::layouts::user_stats::get_size() {
    return (int32_t) this->marshal().get_size();
}
