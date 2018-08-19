#include "../database/tables/user_table.hh"
#include "../utils/play_mode.hh"
#include "ranking_helper.hh"

int32_t shiro::ranking::helper::get_leaderboard_position(uint8_t mode, std::string username) {
    if (username.empty())
        return 0;

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};

    auto result = db(select(all_of(user_table)).from(user_table).unconditionally());
    bool empty = is_query_empty(result);

    if (empty)
        return 0;

    std::vector<std::pair<std::string, float>> users;

    for (const auto &row : result) {
        if ((int) row.id == 0)
            continue;

        switch (mode) {
            case (uint8_t) utils::play_mode::standard:
                users.emplace_back(std::make_pair(row.username, row.pp_std));
                break;
            case (uint8_t) utils::play_mode::taiko:
                users.emplace_back(std::make_pair(row.username, row.pp_taiko));
                break;
            case (uint8_t) utils::play_mode::fruits:
                users.emplace_back(std::make_pair(row.username, row.pp_ctb));
                break;
            case (uint8_t) utils::play_mode::mania:
                users.emplace_back(std::make_pair(row.username, row.pp_mania));
                break;
        }
    }

    std::sort(users.begin(), users.end(), [](const std::pair<std::string, float> &s_left, const std::pair<std::string, float> &s_right) {
        return s_left.second > s_right.second;
    });

    for (size_t i = 0; i < users.size(); i++) {
        std::pair<std::string, float> pair = users.at(i);

        if (username == pair.first)
            return i + 1;
    }

    return 0;
}

std::string shiro::ranking::helper::get_leaderboard_user(uint8_t mode, int32_t pos) {
    if (pos < 1)
        return "";

    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};

    auto result = db(select(all_of(user_table)).from(user_table).unconditionally());
    bool empty = is_query_empty(result);

    if (empty)
        return "";

    std::vector<std::pair<std::string, float>> users;

    for (const auto &row : result) {
        if ((int) row.id == 0)
            continue;

        switch (mode) {
            case (uint8_t) utils::play_mode::standard:
                users.emplace_back(std::make_pair(row.username, row.pp_std));
                break;
            case (uint8_t) utils::play_mode::taiko:
                users.emplace_back(std::make_pair(row.username, row.pp_taiko));
                break;
            case (uint8_t) utils::play_mode::fruits:
                users.emplace_back(std::make_pair(row.username, row.pp_ctb));
                break;
            case (uint8_t) utils::play_mode::mania:
                users.emplace_back(std::make_pair(row.username, row.pp_mania));
                break;
        }
    }

    std::sort(users.begin(), users.end(), [](const std::pair<std::string, float> &s_left, const std::pair<std::string, float> &s_right) {
        return s_left.second > s_right.second;
    });

    try {
        return users.at(pos - 1).first;
    } catch (const std::out_of_range &ex) {
        return "";
    }
}

int16_t shiro::ranking::helper::get_pp_for_user(uint8_t mode, std::string username) {
    sqlpp::mysql::connection db(db_connection->get_config());
    const tables::users user_table {};

    auto result = db(select(all_of(user_table)).from(user_table).where(user_table.username == username));
    bool empty = is_query_empty(result);

    if (empty)
        return 0;

    for (const auto &row : result) {
        if ((int) row.id == 0)
            continue;

        switch (mode) {
            case (uint8_t) utils::play_mode::standard:
                return row.pp_std;
            case (uint8_t) utils::play_mode::taiko:
                return row.pp_taiko;
            case (uint8_t) utils::play_mode::fruits:
                return row.pp_ctb;
            case (uint8_t) utils::play_mode::mania:
                return row.pp_mania;
        }
    }

    return 0;
}
