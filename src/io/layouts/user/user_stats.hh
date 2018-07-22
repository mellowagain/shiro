#ifndef SHIRO_USER_STATS_HH
#define SHIRO_USER_STATS_HH

#include "../serializable.hh"

namespace shiro::io::layouts {

    class user_stats : public serializable {
    public:
        int32_t user_id;

        uint8_t activity;
        std::string activity_desc;

        std::string beatmap_checksum;

        uint32_t current_mods;
        uint8_t play_mode;

        int32_t beatmap_id;

        uint64_t ranked_score;

        float accuracy; // Between 0 and 1 (divide by 100 I guess)
        int32_t play_count;
        uint64_t total_score;

        int32_t rank;
        int16_t pp;

        buffer marshal() override;
        void unmarshal(shiro::io::buffer &buffer) override;

        int32_t get_size() override;

    };

}

#endif //SHIRO_USER_STATS_HH
