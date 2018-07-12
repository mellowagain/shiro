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
    std::pair<std::string, std::string> query_arg;

    if (this->user_id != 0) {
        query_arg = std::make_pair<std::string, std::string>("id", std::to_string(this->user_id));
    } else if (!this->presence.username.empty()) {
        query_arg = std::make_pair<std::string, std::string>("username", "'" + db_connection->escape(this->presence.username) + "'");
    } else {
        LOG_S(ERROR) << "Tried to load user without supplying a valid user id or username.";
        return false;
    }

    char buffer[512];
    std::snprintf(buffer, sizeof(buffer), "SELECT * FROM `users` WHERE %s = %s", query_arg.first.c_str(), query_arg.second.c_str());

    std::vector<MYSQL_ROW> result = db_connection->query(buffer);

    if (result.size() != 1) {
        if (result.size() > 1)
            LOG_F(ERROR, "Got %zu results for user id %i, expected 1.", result.size(), user_id);

        return false;
    }

    MYSQL_ROW row = result.at(0);

    if (!utils::converter::str_to_int(row[0], this->user_id)) {
        LOG_F(ERROR, "Unable to convert user id %s to valid int32_t.", row[0]);
        return false;
    }

    this->presence.user_id = this->user_id;
    this->stats.user_id = this->user_id;
    this->presence.username = row[1];
    this->password = row[3];
    this->salt = row[4];

    if (!utils::converter::str_to_byte(row[10], this->presence.permissions)) {
        LOG_F(WARNING, "Unable to convert permissions %s to valid uint8_t.", row[10]);
        this->presence.permissions = 3; // TODO: Change to user constant
    }

    // On init, set all the data to the standard mode first
    // When the user sends their mode, we load the corresponding data

    if (!utils::converter::str_to_short(row[12], this->stats.pp)) {
        LOG_F(WARNING, "Unable to convert pp %s to valid int16_t.", row[12]);
        this->stats.pp = 0;
    }

    if (!utils::converter::str_to_ulong(row[16], this->stats.total_score)) {
        LOG_F(WARNING, "Unable to convert total score %s to valid uint64_t.", row[16]);
        this->stats.total_score = 0;
    }

    if (!utils::converter::str_to_ulong(row[20], this->stats.ranked_score)) {
        LOG_F(WARNING, "Unable to convert ranked score %s to valid uint64_t.", row[20]);
        this->stats.ranked_score = 0;
    }

    if (!utils::converter::str_to_int(row[24], this->stats.play_count)) {
        LOG_F(WARNING, "Unable to convert play count %s to valid int32_t.", row[24]);
        this->stats.play_count = 0;
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

    return std::string(result) == this->password;
}
