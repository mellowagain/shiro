#include <cstring>

#include "../thirdparty/loguru.hh"
#include "../thirdparty/pbkdf2.hh"
#include "../utils/converter.hh"
#include "user.hh"

shiro::users::user::user(int32_t user_id) : user_id(user_id) {
    // Initialized in initializer list
}

shiro::users::user::user(const std::string &username) {
    this->presence.username = username;
}

bool shiro::users::user::init() {
    using user_struct = std::tuple<int, // id
            std::string, std::string, // username, safe_username
            std::string, std::string, // password, salt
            std::string, std::string, int, int, // email, ip, registration_date, last_seen
            int, int, std::string, // followers, groups, user_page
            float, float, float, float, // pp
            int, int, int, int, // score
            int, int, int, int, // ranked_score
            int, int, int, int, // play_count
            std::string>; // country

    std::vector<user_struct> result {};

    if (this->user_id != 0) {
        result = db_connection->query<user_struct>("SELECT * FROM `users` WHERE id = ?", this->user_id);
    } else if (!this->presence.username.empty()) {
        result = db_connection->query<user_struct>("SELECT * FROM `users` WHERE username = ?", this->presence.username);
    } else {
        LOG_S(ERROR) << "Tried to load user without supplying a valid user id or username.";
        return false;
    }

    if (result.empty())
        return false;

    if (result.size() != 1) {
        if (result.size() > 1)
            LOG_F(ERROR, "Got %zu results for user id %i, expected 1.", result.size(), user_id);

        return false;
    }

    for (const user_struct &user_struct : result) {
        this->user_id = std::get<0>(user_struct);
        this->presence.user_id = this->user_id;
        this->stats.user_id = this->user_id;
        this->presence.username = std::get<1>(user_struct);
        this->password = std::get<3>(user_struct);
        this->salt = std::get<4>(user_struct);
        this->presence.permissions = std::get<10>(user_struct);
        this->stats.pp = std::get<12>(user_struct);
        this->stats.total_score = std::get<16>(user_struct);
        this->stats.ranked_score = std::get<20>(user_struct);
        this->stats.play_count = std::get<24>(user_struct);
    }

    return true;
}

void shiro::users::user::update() {
    // TODO: Update stats of user based on current mode etc.
}

bool shiro::users::user::check_password(const std::string &password) {
    if (this->password.empty() || password.empty())
        return false;

    char result[129];
    std::memset(result, 0, sizeof(result));

    uint8_t binary_result[64];
    std::memset(binary_result, 0, sizeof(binary_result));

    PBKDF2_HMAC_SHA_512(
            password.c_str(), password.length(),
            reinterpret_cast<const unsigned char*>(this->salt.c_str()), this->salt.length(),
            4096, 64, result, binary_result
    );

    return std::string(password) == this->password;
}
