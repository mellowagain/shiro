#include "../../../scores/score.hh"
#include "../../../scores/score_helper.hh"
#include "../../../utils/osu_string.hh"
#include "../../../utils/play_mode.hh"
#include "user_stats.hh"

void shiro::io::layouts::user_stats::recalculate_accuracy() {
    std::vector<scores::score> scores = scores::helper::fetch_all_user_scores(this->user_id);

    switch (this->play_mode) {
        case (uint8_t) utils::play_mode::standard: {
            scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const scores::score &s) {
                return s.play_mode != (uint8_t) utils::play_mode::standard;
            }), scores.end());
            break;
        }
        case (uint8_t) utils::play_mode::taiko: {
            scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const scores::score &s) {
                return s.play_mode != (uint8_t) utils::play_mode::taiko;
            }), scores.end());
            break;
        }
        case (uint8_t) utils::play_mode::fruits: {
            scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const scores::score &s) {
                return s.play_mode != (uint8_t) utils::play_mode::fruits;
            }), scores.end());
            break;
        }
        case (uint8_t) utils::play_mode::mania: {
            scores.erase(std::remove_if(scores.begin(), scores.end(), [&](const scores::score &s) {
                return s.play_mode != (uint8_t) utils::play_mode::mania;
            }), scores.end());
            break;
        }
    }

    float accuracy = 0.0f;

    for (const scores::score &score : scores) {
        accuracy += score.accuracy;
    }

    this->accuracy = (accuracy / scores.size()) / 100;
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

    buf.write<float>(this->accuracy);
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

    this->accuracy = buffer.read<float>();
    this->play_count = buffer.read<int32_t>();
    this->total_score = buffer.read<uint64_t>();

    this->rank = buffer.read<int32_t>();
    this->pp = buffer.read<int16_t>();
}

int32_t shiro::io::layouts::user_stats::get_size() {
    return (int32_t) this->marshal().get_size();
}
