#ifndef SHIRO_SCORE_HELPER_HH
#define SHIRO_SCORE_HELPER_HH

#include <memory>
#include <string>
#include <vector>

#include "../users/user.hh"
#include "score.hh"

namespace shiro::scores::helper {

    score fetch_top_score_user(std::string beatmap_md5sum, std::shared_ptr<users::user> user);

    std::vector<score> fetch_all_scores(std::string beatmap_md5sum, size_t limit = 50);

    std::vector<score> fetch_country_scores(std::string beatmap_md5sum, uint8_t country, size_t limit = 50);

    std::vector<score> fetch_mod_scores(std::string beatmap_md5sum, int32_t mods, size_t limit = 50);

    std::vector<score> fetch_friend_scores(std::string beatmap_md5sum, std::shared_ptr<users::user> user, size_t limit = 50);

    int32_t get_scoreboard_position(const score &s);

}

#endif //SHIRO_SCORE_HELPER_HH
