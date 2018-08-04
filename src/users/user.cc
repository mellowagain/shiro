#include <cstring>

#include "../database/tables/relationship_table.hh"
#include "../database/tables/user_table.hh"
#include "../thirdparty/loguru.hh"
#include "../thirdparty/pbkdf2.hh"
#include "../utils/play_mode.hh"
#include "../shiro.hh"
#include "user.hh"

shiro::users::user::user(int32_t user_id) : user_id(user_id) {
    // Initialized in initializer list
}

shiro::users::user::user(const std::string &username) {
    this->presence.username = username;
}

bool shiro::users::user::init() {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};
    const tables::relationships relationships_table {};

    auto user_result = db(select(all_of(user_table)).from(user_table).where(user_table.id == this->user_id or user_table.username == this->presence.username));
    bool user_result_empty = is_query_empty(user_result);

    if (user_result_empty) {
        LOG_S(ERROR) << "Result is empty.";
        return false;
    }

    for (const auto &row : user_result) {
        this->user_id = row.id;
        this->presence.user_id = this->user_id;
        this->stats.user_id = this->user_id;
        this->presence.username = row.username;
        this->password = row.password;
        this->salt = row.salt;
        this->presence.permissions = row.groups;
        this->stats.pp = row.pp_std;
        this->stats.total_score = row.score_std;
        this->stats.ranked_score = row.ranked_score_std;
        this->stats.play_count = row.play_count_std;
    }

    auto relationship_result = db(select(all_of(relationships_table)).from(relationships_table).where(relationships_table.origin == this->user_id and relationships_table.blocked == false));

    for (const auto &row : relationship_result) {
        this->friends.emplace_back(row.target);
    }

    return true;
}

void shiro::users::user::update() {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};

    auto result = db(select(all_of(user_table)).from(user_table).where(user_table.id == this->user_id));
    bool empty = is_query_empty(result);

    if (empty)
        return;

    for (const auto &row : result) {
        uint8_t mode = this->status.play_mode;

        if (mode == (uint8_t) utils::play_mode::standard) {
            this->stats.pp = row.pp_std;
            this->stats.total_score = row.score_std;
            this->stats.ranked_score = row.ranked_score_std;
            this->stats.play_count = row.play_count_std;
        } else if (mode == (uint8_t) utils::play_mode::taiko) {
            this->stats.pp = row.pp_taiko;
            this->stats.total_score = row.score_taiko;
            this->stats.ranked_score = row.ranked_score_taiko;
            this->stats.play_count = row.play_count_taiko;
        } else if (mode == (uint8_t) utils::play_mode::fruits) {
            this->stats.pp = row.pp_ctb;
            this->stats.total_score = row.score_ctb;
            this->stats.ranked_score = row.ranked_score_ctb;
            this->stats.play_count = row.play_count_ctb;
        } else if (mode == (uint8_t) utils::play_mode::mania) {
            this->stats.pp = row.pp_mania;
            this->stats.total_score = row.score_mania;
            this->stats.ranked_score = row.ranked_score_mania;
            this->stats.play_count = row.play_count_mania;
        }
    }
}

bool shiro::users::user::check_password(const std::string &password) {
    if (this->password.empty() || password.empty())
        return false;

    char result[128];
    std::memset(result, 0, sizeof(result));

    uint8_t binary_result[64];
    std::memset(binary_result, 0, sizeof(binary_result));

    PBKDF2_HMAC_SHA_512(
            password.c_str(), (int) password.length(),
            reinterpret_cast<const unsigned char*>(this->salt.c_str()), (int) this->salt.length(),
            4096, 64, result, binary_result
    );

    return std::string(result) == this->password;
}
