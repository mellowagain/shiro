#ifndef SHIRO_QUEUE_HH
#define SHIRO_QUEUE_HH

#include <string>

#include "osu_buffer.hh"
#include "osu_writer.hh"

namespace shiro::io {

    class queue {
    private:
        io::buffer packet_queue;

    public:
        queue() = default;

        void enqueue(osu_writer &writer);
        void enqueue(buffer &buffer);

        bool is_empty();

        // This function additionally clears the queue
        std::string serialize();

    };

}

#endif //SHIRO_QUEUE_HH
