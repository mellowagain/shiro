#ifndef SHIRO_BEATMAP_HH
#define SHIRO_BEATMAP_HH

#include <cstdint>
#include <string>

#include "../utils/play_mode.hh"

namespace shiro::beatmaps {

    class beatmap {
    public:
        int32_t id = 0;
        int32_t beatmap_id = 0;
        int32_t beatmapset_id = 0;
        uint8_t play_mode = (uint8_t) utils::play_mode::standard;

        std::string beatmap_md5 = "";
        std::string song_name = "";

        float ar = 0.0f;
        float od = 0.0f;

        float diff_std = 0.0f;
        float diff_taiko = 0.0f;
        float diff_ctb = 0.0f;
        float diff_mania = 0.0f;

        int32_t max_combo = 0;
        int32_t hit_length = 0;
        int32_t bpm = 0;

        int32_t ranked_status = -2;
        bool ranked_status_freezed = false;

        int32_t last_update = 0;

        int32_t play_count = 0;
        int32_t pass_count = 0;

        void fetch(bool force_peppster = false);
        bool fetch_db();

        bool fetch_api();

        void update_play_metadata();

        std::string build_header();

    };

}

#endif //SHIRO_BEATMAP_HH
