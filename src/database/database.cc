#include "database.hh"

shiro::database::database(const std::string &address, unsigned int port, const std::string &db, const std::string &username, const std::string &password)
    : address(address)
    , port(port)
    , db(db)
    , username(username)
    , password(password) {
    // Initialized in initializer list
}

void shiro::database::connect() {
    if (this->is_connected())
        return;

    this->connection = std::make_shared<MySql>(
            this->address.c_str(),
            this->username.c_str(), this->password.c_str(),
            this->db.c_str(), (uint16_t) this->port
    );

    LOG_S(INFO) << "Successfully connected to MySQL database.";
}

void shiro::database::setup() {
    if (!this->is_connected())
        return;

    // Beatmaps
    update("CREATE TABLE IF NOT EXISTS `beatmaps` "
           "(id INT PRIMARY KEY NOT NULL AUTO_INCREMENT, "
           "beatmap_id INT, beatmapset_id INT, beatmap_md5 VARCHAR(35), song_name VARCHAR(128), "
           "ar FLOAT, od FLOAT, diff_std FLOAT, diff_taiko FLOAT, diff_ctb FLOAT, diff_mania FLOAT, "
           "max_combo INT, hit_length INT, bpm INT, ranked TINYINT, last_update INT, "
           "ranked_status_freezed BOOLEAN, play_count INT, pass_count INT);");

    // IRC channels
    update("CREATE TABLE IF NOT EXISTS `channels` "
           "(id INT PRIMARY KEY NOT NULL AUTO_INCREMENT, "
           "name VARCHAR(32), description VARCHAR(64), auto_join BOOLEAN);");

    // Submitted scores
    update("CREATE TABLE IF NOT EXISTS `scores` "
           "(id INT PRIMARY KEY NOT NULL AUTO_INCREMENT, "
           "beatmap_md5 VARCHAR(35), user_id INT, score BIGINT, max_combo INT, fc BOOLEAN, mods INT, "
           "300_count INT, 100_count INT, 50_count INT, "
           "katus_count INT, gekis_count INT, miss_count INT, "
           "time INT, play_mode TINYINT, passed BOOLEAN, accuracy FLOAT, pp FLOAT);");

    // Registered users and their stats
    update("CREATE TABLE IF NOT EXISTS `users` (id INT PRIMARY KEY NOT NULL AUTO_INCREMENT, "
           "username VARCHAR(32), safe_username VARCHAR(32), "
           "password VARCHAR(128), salt VARCHAR(64),"
           "email VARCHAR(100), ip VARCHAR(39), registration_date INT, last_seen INT, "
           "followers INT, groups INT, user_page TEXT, "
           "pp_std FLOAT, pp_taiko FLOAT, pp_ctb FLOAT, pp_mania FLOAT, "
           "score_std INT, score_taiko INT, score_ctb INT, score_mania INT, "
           "ranked_score_std INT, ranked_score_taiko INT, ranked_score_ctb INT, ranked_score_mania INT, "
           "play_count_std INT, play_count_taiko INT, play_count_ctb INT, play_count_mania INT, "
           "country VARCHAR(2));");

    LOG_S(INFO) << "Successfully created and structured tables in database.";
}

bool shiro::database::is_connected() {
    return this->connection != nullptr;
}

std::shared_ptr<MySql> shiro::database::get_connection() {
    if (!is_connected())
        return nullptr;

    return this->connection;
}
