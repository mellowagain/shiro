#include "queue.hh"

void shiro::io::queue::enqueue(shiro::io::osu_writer &writer) {
    buffer buf = writer.get_buffer();
    this->packet_queue.write_buffer(buf);
}

void shiro::io::queue::enqueue(shiro::io::buffer &buffer) {
    this->packet_queue.write_buffer(buffer);
}

bool shiro::io::queue::is_empty() {
    return this->packet_queue.is_empty();
}

std::string shiro::io::queue::serialize() {
    std::string result = this->packet_queue.serialize();
    this->packet_queue.reset();

    return result;
}
