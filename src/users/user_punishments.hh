#ifndef SHIRO_USER_PUNISHMENTS_HH
#define SHIRO_USER_PUNISHMENTS_HH

#include <memory>
#include <string>
#include <tuple>

#include "../config/bot_file.hh"

namespace shiro::users::punishments {

    void init();

    void kick(int32_t user_id, const std::string &reason = "You have been kicked. Please login again.");
    void silence(int32_t user_id, uint32_t duration, const std::string &reason = "Silenced (" + config::bot::name + " protection)");
    void restrict(int32_t user_id, const std::string &reason = "Your account has been flagged.");
    void ban(int32_t user_id, const std::string &reason = "Your account has been convicted.");

    bool is_silenced(int32_t user_id);
    bool is_restricted(int32_t user_id);
    bool is_banned(int32_t user_id);

    bool can_chat(int32_t user_id);
    bool has_scores(int32_t user_id);

    std::tuple<int32_t, uint32_t> get_silence_time(int32_t user_id);

}

#endif //SHIRO_USER_PUNISHMENTS_HH
