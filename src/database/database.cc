#include "../thirdparty/loguru.hh"
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
    if (is_connected())
        return;

    this->connection = mysqlpp::Connection(false);

    if (!this->connection.connect(this->db.c_str(), this->address.c_str(), this->username.c_str(), this->password.c_str(), this->port)) {
        LOG_S(FATAL) << "Unable to connect to MySQL database: " << this->connection.error() << ".";
        std::exit(-1);
    }

    LOG_S(INFO) << "Successfully connected to database.";
}

void shiro::database::disconnect() {
    if (!is_connected())
        return;

    this->connection.disconnect();
    LOG_S(INFO) << "Successfully disconnected from database.";
}

void shiro::database::setup() {
    // Beatmaps
    update("CREATE TABLE IF NOT EXISTS `beatmaps` "
           "(id INT PRIMARY KEY, beatmap_id INT, beatmapset_id INT, beatmap_md5 VARCHAR(32), song_name VARCHAR(128), "
           "ar FLOAT, od FLOAT, diff_std FLOAT, diff_taiko FLOAT, diff_ctb FLOAT, diff_mania FLOAT, "
           "max_combo INT, hit_length INT, bpm INT, ranked BOOLEAN, last_update INT, "
           "ranked_status_freezed BOOLEAN, play_count INT, pass_count INT, "
           "pp_100 INT, pp_99 INT, pp_98 INT, pp_95 INT);");

    // IRC channels
    update("CREATE TABLE IF NOT EXISTS `channels` (id INT, name VARCHAR(32), desc VARCHAR(32), is_staff_only BOOL);");

    // Submitted scores
    update("CREATE TABLE IF NOT EXISTS `scores` "
           "(id INT PRIMARY KEY, beatmap_md5 VARCHAR(32), user_id INT, score BIGINT, max_combo INT, fc BOOLEAN, mods INT, "
           "300_count INT, 100_count INT, 50_count INT, "
           "katus_count INT, gekis_count INT, miss_count INT, "
           "time VARCHAR(18), play_mode TINYINT, passed BOOLEAN, accuracy FLOAT, pp FLOAT);");

    // Registered users and their stats
    update("CREATE TABLE IF NOT EXISTS `users` (id INT PRIMARY KEY, username VARCHAR(255), safe_username VARCHAR(255), "
           "password VARCHAR(64), email VARCHAR(100), ip VARCHAR(50), registration_date INT, last_seen INT, "
           "followers INT, groups INT, user_page TEXT, "
           "pp_std FLOAT, pp_taiko FLOAT, pp_ctb FLOAT, pp_mania FLOAT, "
           "score_std INT, score_taiko INT, score_ctb INT, score_mania INT, "
           "ranked_score_std INT, ranked_score_taiko INT, ranked_score_ctb INT, ranked_score_mania INT, "
           "play_count_std INT, play_count_taiko INT, play_count_ctb INT, play_count_mania INT, "
           "country VARCHAR(2));");
}

void shiro::database::update(const std::string &query_str) {
    if (!is_connected())
        return;

    mysqlpp::Query query = this->connection.query(query_str);
    mysqlpp::StoreQueryResult result = query.store();

    if (result == nullptr)
        LOG_S(ERROR) << "Failed to execute database update: " << query.error() << ".";
}

mysqlpp::StoreQueryResult shiro::database::query(const std::string &query_str) {
    if (!is_connected())
        return mysqlpp::StoreQueryResult();

    mysqlpp::Query query = this->connection.query(query_str);
    mysqlpp::StoreQueryResult result = query.store();

    if (result == nullptr)
        LOG_S(ERROR) << "Failed to execute database query: " << query.error() << ".";

    return result;
}

bool shiro::database::is_connected() {
    return this->connection != nullptr && this->connection.connected();
}

mysqlpp::Connection shiro::database::get_connection() {
    return this->connection;
}
