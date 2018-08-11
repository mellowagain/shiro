#ifndef SHIRO_SCORE_HH
#define SHIRO_SCORE_HH

#include <cstdint>
#include <string>

#include "../utils/play_mode.hh"

namespace shiro::scores {

    class score {
    public:
        score() = default;

        explicit score(int32_t id) : id(id) {
            // Initialized in initializer list
        }

        int32_t id = 0;
        int32_t user_id = 0;
        std::string beatmap_md5 = "";

        int64_t total_score = 0;
        int32_t max_combo = 0;
        float pp = 0.0f;

        float accuracy = 0.0f;
        int32_t mods = 0;

        bool fc = false;
        bool passed = false;

        int32_t _300_count = 0;
        int32_t _100_count = 0;
        int32_t _50_count = 0;
        int32_t katus_count = 0;
        int32_t gekis_count = 0;
        int32_t miss_count = 0;

        uint8_t play_mode = (uint8_t) utils::play_mode::standard;
        int32_t time = 0;

        std::string to_string();

    };

}

#endif //SHIRO_SCORE_HH
