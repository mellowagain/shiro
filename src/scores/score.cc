#include <sstream>
#include <memory>

#include "score.hh"
#include "../users/user_manager.hh"
#include "../users/user.hh"
#include "score_helper.hh"

std::string shiro::scores::score::to_string(std::vector<score> &scores) {
    std::stringstream stream;
    std::shared_ptr<users::user> user = users::manager::get_user_by_id(this->user_id);

    if (user == nullptr) {
        user = std::make_shared<users::user>(this->user_id);
        if (!user->init())
            return "";
    }

    stream << this->id << "|";
    stream << user->presence.username << "|";
    stream << this->total_score << "|";
    stream << this->max_combo << "|";
    stream << this->_50_count << "|";
    stream << this->_100_count << "|";
    stream << this->_300_count << "|";
    stream << this->miss_count << "|";
    stream << this->katus_count << "|";
    stream << this->gekis_count << "|";
    stream << (this->fc ? "True" : "False") << "|";
    stream << this->mods << "|";
    stream << this->user_id << "|";
    stream << helper::get_scoreboard_position(*this, scores) << "|";
    stream << this->time << "|";
    stream << "0" << std::endl;

    return stream.str();
}
