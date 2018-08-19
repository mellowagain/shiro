#include "../config/cli_args.hh"
#include "../thirdparty/loguru.hh"
#include "../shiro.hh"
#include "database.hh"

shiro::database::database(const std::string &address, unsigned int port, const std::string &db, const std::string &username, const std::string &password)
    : address(address)
    , port(port)
    , db(db)
    , username(username)
    , password(password) {
    auto [argc, argv] = config::cli::get_args();
    sqlpp::mysql::global_library_init(argc, argv);
}

void shiro::database::connect() {
    if (this->is_connected())
        return;

    this->config = std::make_shared<sqlpp::mysql::connection_config>();
    this->config->host = this->address;
    this->config->port = this->port;
    this->config->database = this->db;
    this->config->user = this->username;
    this->config->password = this->password;
    this->config->auto_reconnect = true;

    LOG_S(INFO) << "Successfully connected to MySQL database.";
}

void shiro::database::setup() {
    if (!this->is_connected())
        return;

    sqlpp::mysql::connection db(this->config);

    // Beatmaps
    db.execute(
           "CREATE TABLE IF NOT EXISTS `beatmaps` "
           "(id INT PRIMARY KEY NOT NULL AUTO_INCREMENT, "
           "beatmap_id INT NOT NULL, beatmapset_id INT NOT NULL, beatmap_md5 VARCHAR(35) NOT NULL, song_name VARCHAR(128) NOT NULL, "
           "game_mode SMALLINT NOT NULL, ar FLOAT NOT NULL, od FLOAT NOT NULL, diff_std FLOAT NOT NULL, diff_taiko FLOAT NOT NULL, diff_ctb FLOAT NOT NULL, diff_mania FLOAT NOT NULL, "
           "max_combo INT NOT NULL, hit_length INT NOT NULL, bpm INT NOT NULL, ranked_status INT NOT NULL, last_update INT NOT NULL, "
           "ranked_status_freezed BOOLEAN NOT NULL, play_count INT NOT NULL, pass_count INT NOT NULL);"
    );

    // IRC channels
    db.execute(
           "CREATE TABLE IF NOT EXISTS `channels` "
           "(id INT PRIMARY KEY NOT NULL AUTO_INCREMENT, "
           "name VARCHAR(32) NOT NULL, description VARCHAR(64) NOT NULL, auto_join BOOLEAN NOT NULL);"
    );

    // Submitted scores
    db.execute(
           "CREATE TABLE IF NOT EXISTS `scores` "
           "(id INT PRIMARY KEY NOT NULL AUTO_INCREMENT, "
           "beatmap_md5 VARCHAR(35) NOT NULL, hash VARCHAR(35) NOT NULL, user_id INT NOT NULL, rank VARCHAR(2) NOT NULL, "
           "score BIGINT NOT NULL, max_combo INT NOT NULL, fc BOOLEAN NOT NULL, mods INT NOT NULL, "
           "300_count INT NOT NULL, 100_count INT NOT NULL, 50_count INT NOT NULL, "
           "katus_count INT NOT NULL, gekis_count INT NOT NULL, miss_count INT NOT NULL, "
           "time INT NOT NULL, play_mode TINYINT NOT NULL, passed BOOLEAN NOT NULL, accuracy FLOAT NOT NULL, pp FLOAT NOT NULL);"
    );

    // Registered users and their stats
    db.execute(
           "CREATE TABLE IF NOT EXISTS `users` "
           "(id INT PRIMARY KEY NOT NULL AUTO_INCREMENT, "
           "username VARCHAR(32) NOT NULL, safe_username VARCHAR(32) NOT NULL, "
           "password VARCHAR(128) NOT NULL, salt VARCHAR(64) NOT NULL,"
           "email VARCHAR(100) NOT NULL, ip VARCHAR(39) NOT NULL, registration_date INT NOT NULL, last_seen INT NOT NULL, "
           "followers INT NOT NULL, groups INT NOT NULL, user_page TEXT NOT NULL, "
           "pp_std FLOAT NOT NULL, pp_taiko FLOAT NOT NULL, pp_ctb FLOAT NOT NULL, pp_mania FLOAT NOT NULL, "
           "rank_std INT NOT NULL, rank_taiko INT NOT NULL, rank_ctb INT NOT NULL, rank_mania INT NOT NULL, "
           "score_std INT NOT NULL, score_taiko INT NOT NULL, score_ctb INT NOT NULL, score_mania INT NOT NULL, "
           "ranked_score_std INT NOT NULL, ranked_score_taiko INT NOT NULL, ranked_score_ctb INT NOT NULL, ranked_score_mania INT NOT NULL, "
           "accuracy_std FLOAT NOT NULL, accuracy_taiko FLOAT NOT NULL, accuracy_ctb FLOAT NOT NULL, accuracy_mania FLOAT NOT NULL,"
           "play_count_std INT NOT NULL, play_count_taiko INT NOT NULL, play_count_ctb INT NOT NULL, play_count_mania INT NOT NULL, "
           "country VARCHAR(2) NOT NULL);"
    );

    // Relationships between users (friends and blocked)
    db.execute(
           "CREATE TABLE IF NOT EXISTS `relationships` (origin INT NOT NULL, target INT NOT NULL, blocked BOOLEAN NOT NULL);"
    );

    LOG_S(INFO) << "Successfully created and structured tables in database.";
}

bool shiro::database::is_connected() {
    return this->config != nullptr;
}

std::shared_ptr<sqlpp::mysql::connection_config> shiro::database::get_config() {
    return this->config;
}
