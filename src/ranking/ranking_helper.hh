#ifndef SHIRO_RANKING_HELPER_HH
#define SHIRO_RANKING_HELPER_HH

#include <cstdint>
#include <memory>
#include <string>

#include "../users/user.hh"

namespace shiro::ranking::helper {

    int32_t get_leaderboard_position(uint8_t mode, std::string username);

    std::string get_leaderboard_user(uint8_t mode, int32_t pos);

    int16_t get_pp_for_user(uint8_t mode, std::string username);

}

#endif //SHIRO_RANKING_HELPER_HH
