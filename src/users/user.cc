/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2019 Marc3842h, czapek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "../config/ipc_file.hh"
#include "../database/tables/relationship_table.hh"
#include "../database/tables/user_stats_table.hh"
#include "../database/tables/user_table.hh"
#include "../permissions/role_manager.hh"
#include "../thirdparty/loguru.hh"
#include "../utils/crypto.hh"
#include "user.hh"
#include "user_punishments.hh"

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

    auto user_result = db(select(all_of(user_table)).from(user_table).where(user_table.id == this->user_id or user_table.username == this->presence.username).limit(1u));

    if (user_result.empty())
        return false;

    const auto &row = user_result.front();

    this->user_id = row.id;
    this->presence.user_id = this->user_id;
    this->stats.user_id = this->user_id;
    this->presence.username = row.username;
    this->password = row.password;
    this->salt = row.salt;
    this->roles = row.roles;
    this->presence.permissions = roles::manager::get_chat_color(this->roles);
    this->country = row.country;

    this->update();

    auto relationship_result = db(select(all_of(relationships_table)).from(relationships_table).where(relationships_table.origin == this->user_id and relationships_table.blocked == false));

    for (const auto &row : relationship_result) {
        this->friends.emplace_back(row.target);
    }

    if (users::punishments::is_restricted(this->user_id))
        this->hidden = true;

    return true;
}

void shiro::users::user::update() {
    sqlpp::mysql::connection db(db_connection->get_config());

    #if defined(SEPARATE_RX_LEADERBOARDS)
        if (this->is_relax()) {
            const tables::user_stats_rx stats_table {};
            auto result = db(select(all_of(stats_table)).from(stats_table).where(
                    stats_table.id == this->user_id
            ).limit(1u));

            if (result.empty()) {
                db(insert_into(stats_table).set(stats_table.id = this->user_id));
                return;
            }

            const auto &row = result.front();
            this->fill_local_stats<decltype(row)>(row);
            return;
        }
    #endif

    #if defined(SEPARATE_AP_LEADERBOARDS)
        if (this->is_auto_pilot()) {
            const tables::user_stats_ap stats_table {};
            auto result = db(select(all_of(stats_table)).from(stats_table).where(
                    stats_table.id == this->user_id
            ).limit(1u));

            if (result.empty()) {
                db(insert_into(stats_table).set(stats_table.id = this->user_id));
                return;
            }

            const auto &row = result.front();
            this->fill_local_stats<decltype(row)>(row);
            return;
        }
    #endif

    const tables::user_stats stats_table {};
    auto result = db(select(all_of(stats_table)).from(stats_table).where(
            stats_table.id == this->user_id
    ).limit(1u));

    if (result.empty()) {
        db(insert_into(stats_table).set(stats_table.id = this->user_id));
        return;
    }

    const auto &row = result.front();
    this->fill_local_stats<decltype(row)>(row);
}

void shiro::users::user::save_stats() {
    #define play_mode_switch(table)                                                 \
        switch (this->stats.play_mode) {                                            \
            case (uint8_t) utils::play_mode::standard:                              \
                db(sqlpp::update(stats_table).set(                                  \
                    stats_table.pp_std = this->stats.pp,                            \
                    stats_table.score_std = this->stats.total_score,                \
                    stats_table.ranked_score_std = this->stats.ranked_score,        \
                    stats_table.play_count_std = this->stats.play_count,            \
                    stats_table.rank_std = this->stats.rank,                        \
                    stats_table.max_combo_std = this->stats.max_combo,              \
                    stats_table.accuracy_std = this->stats.accuracy                 \
                ).where(stats_table.id == this->user_id));                          \
                break;                                                              \
            case (uint8_t) utils::play_mode::taiko:                                 \
                db(sqlpp::update(stats_table).set(                                  \
                    stats_table.pp_taiko = this->stats.pp,                          \
                    stats_table.score_taiko = this->stats.total_score,              \
                    stats_table.ranked_score_taiko = this->stats.ranked_score,      \
                    stats_table.play_count_taiko = this->stats.play_count,          \
                    stats_table.rank_taiko = this->stats.rank,                      \
                    stats_table.max_combo_taiko = this->stats.max_combo,            \
                    stats_table.accuracy_taiko = this->stats.accuracy               \
                ).where(stats_table.id == this->user_id));                          \
                break;                                                              \
            case (uint8_t) utils::play_mode::fruits:                                \
                db(sqlpp::update(stats_table).set(                                  \
                    stats_table.pp_ctb = this->stats.pp,                            \
                    stats_table.score_ctb = this->stats.total_score,                \
                    stats_table.ranked_score_ctb = this->stats.ranked_score,        \
                    stats_table.play_count_ctb = this->stats.play_count,            \
                    stats_table.rank_ctb = this->stats.rank,                        \
                    stats_table.max_combo_ctb = this->stats.max_combo,              \
                    stats_table.accuracy_ctb = this->stats.accuracy                 \
                ).where(stats_table.id == this->user_id));                          \
                break;                                                              \
            case (uint8_t) utils::play_mode::mania:                                 \
                db(sqlpp::update(stats_table).set(                                  \
                    stats_table.pp_mania = this->stats.pp,                          \
                    stats_table.score_mania = this->stats.total_score,              \
                    stats_table.ranked_score_mania = this->stats.ranked_score,      \
                    stats_table.play_count_mania = this->stats.play_count,          \
                    stats_table.rank_mania = this->stats.rank,                      \
                    stats_table.max_combo_mania = this->stats.max_combo,            \
                    stats_table.accuracy_mania = this->stats.accuracy               \
                ).where(stats_table.id == this->user_id));                          \
                break;                                                              \
        }

    sqlpp::mysql::connection db(db_connection->get_config());

    #if defined(SEPARATE_RX_LEADERBOARDS)
        if (this->is_relax()) {
            const tables::user_stats_rx stats_table {};

            play_mode_switch(stats_table)
            return;
        }
    #endif

    #if defined(SEPARATE_AP_LEADERBOARDS)
        if (this->is_auto_pilot()) {
            const tables::user_stats_ap stats_table {};

            play_mode_switch(stats_table)
            return;
        }
    #endif

    const tables::user_stats stats_table {};

    play_mode_switch(stats_table)

    #undef play_mode_switch
}

#if defined(SEPARATE_RX_LEADERBOARDS)
    bool shiro::users::user::is_relax() {
        return this->stats.current_mods & (uint32_t) utils::mods::relax;
    }
#endif

#if defined(SEPARATE_AP_LEADERBOARDS)
    bool shiro::users::user::is_auto_pilot() {
        return this->stats.current_mods & (uint32_t) utils::mods::auto_pilot;
    }
#endif

std::string shiro::users::user::get_url() {
    static std::string url = config::ipc::frontend_url + "u/" + std::to_string(this->user_id);
    return url;
}

void shiro::users::user::refresh_stats() {
    io::osu_writer writer;

    writer.user_stats(this->stats);
    writer.user_presence(this->presence);

    this->queue.enqueue(writer);
}

bool shiro::users::user::check_password(const std::string &password) {
    if (this->password.empty() || password.empty())
        return false;

    return utils::crypto::pbkdf2_hmac_sha512::hash(password, this->salt) == this->password;
}
