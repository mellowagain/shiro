#include "spectate_frames.hh"

shiro::io::buffer shiro::io::layouts::spectate_frames::marshal() {
    buffer buf;

    buf.write<int32_t>(this->extra);

    buf.write<int16_t>(this->replay_frames.size());

    for (replay_frame frame : this->replay_frames) {
        buffer replay_frame_buf = frame.marshal();
        buf.append(replay_frame_buf);
    }

    buf.write<uint8_t>(this->action);

    buffer score_frame_buf = this->score_frame.marshal();
    buf.append(score_frame_buf);

    return buf;
}

void shiro::io::layouts::spectate_frames::unmarshal(shiro::io::buffer &buffer) {
    this->extra = buffer.read<int32_t>();

    int16_t size = buffer.read<int16_t>();
    this->replay_frames.reserve(size);

    for (int16_t i = 0; i < size; i++) {
        replay_frame frame;
        frame.unmarshal(buffer);

        this->replay_frames.emplace_back(frame);
    }

    this->action = buffer.read<uint8_t>();

    layouts::score_frame frame;
    frame.unmarshal(buffer);

    this->score_frame = frame;
}

int32_t shiro::io::layouts::spectate_frames::get_size() {
    return this->marshal().get_size();
}
