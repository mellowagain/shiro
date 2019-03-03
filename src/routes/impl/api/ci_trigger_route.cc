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

#include <bitset>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../../../config/api_file.hh"
#include "../../../thirdparty/loguru.hh"
#include "../../../users/user.hh"
#include "../../../users/user_manager.hh"
#include "../../../utils/filesystem.hh"
#include "../../../utils/multipart_parser.hh"
#include "ci_trigger_route.hh"

void shiro::routes::api::ci_trigger::handle(const crow::request &request, crow::response &response) {
    if (!config::api::deploy_enabled) {
        response.code = 404;
        response.end();
        return;
    }

    response.set_header("Content-Type", "text/plain; charset=UTF-8");
    response.set_header("cho-server", "shiro (https://github.com/Marc3842h/shiro)");

    const std::string &user_agent = request.get_header_value("user-agent");

    if (user_agent.empty() || user_agent.find("CI") == std::string::npos) {
        response.code = 403;
        response.end();

        LOG_F(WARNING, "Received CI trigger from %s without a CI user agent.", request.get_ip_address().c_str());
        return;
    }

    const std::string &content_type = request.get_header_value("content-type");

    if (content_type.empty()) {
        response.code = 400;
        response.end();

        LOG_F(WARNING, "Received CI trigger from %s without content type.", request.get_ip_address().c_str());
        return;
    }

    std::unique_ptr<utils::multipart_parser> parser = std::make_unique<utils::multipart_parser>(request.body, content_type);

    if (parser == nullptr)
        return;

    utils::multipart_form_fields fields = parser->parse_fields();

    if (fields.find("token") == fields.end() || fields.at("token").type != utils::multipart_field_type::text) {
        response.code = 403;
        response.end();

        LOG_F(WARNING, "Received CI trigger from %s without token.", request.get_ip_address().c_str());
        return;
    }

    if (fields.find("commit") == fields.end() || fields.at("commit").type != utils::multipart_field_type::text) {
        response.code = 400;
        response.end();

        LOG_F(WARNING, "Received CI trigger from %s without commit SHA1.", request.get_ip_address().c_str());
        return;
    }

    if (fields.find("shiro") == fields.end() || fields.at("shiro").type != utils::multipart_field_type::file) {
        response.code = 400;
        response.end();

        LOG_F(WARNING, "Received CI trigger from %s without attached file.", request.get_ip_address().c_str());
        return;
    }

    std::string token = fields.at("token").body;

    if (token != config::api::deploy_key) {
        response.code = 403;
        response.end();

        LOG_F(WARNING, "Received CI trigger from %s with invalid token (%s).", request.get_ip_address().c_str(), token.c_str());
        return;
    }

    response.code = 200;
    response.end();

    // TODO: Replace hardcoded name with argv[0]
    fs::path shiro_executable = fs::current_path() / "shiro";

    if (fs::exists(shiro_executable)) {
        // Remove file only if it exists
        if (fs::remove(shiro_executable)) {
            LOG_F(ERROR, "Shiro was unable to delete old version: %s (%i)", std::strerror(errno), errno);
            return;
        }
    }

    std::ofstream stream("shiro", std::ios::binary | std::ios::trunc);
    stream << fields.at("shiro").body;
    stream.close();

    fs::permissions(
            shiro_executable,
            fs::perms::owner_all | fs::perms::group_exec | fs::perms::others_exec,
            fs::perm_options::add
    );

    std::string short_hash = fields.at("commit").body.substr(0, 7);

    std::ofstream commit_stream("commit.txt", std::ios::trunc);
    commit_stream << short_hash;
    commit_stream.close();

    LOG_F(INFO, "A new version of Shiro has been deployed by CI (%s).", short_hash.c_str());

    io::osu_writer announce_writer;
    announce_writer.announce(
            "Shiro is deploying a new version (" + short_hash + "). "
            "You will be automatically reconnected in a few seconds."
    );

    io::osu_writer restart_writer;
    restart_writer.bancho_restart(10000);

    for (const std::shared_ptr<users::user> &user : users::manager::online_users) {
        if (user == nullptr || user->user_id == 1)
            continue;

        user->queue.enqueue(announce_writer);
        user->queue.enqueue_next(restart_writer);
    }

    std::thread deploy_thread([]() {
        std::this_thread::sleep_for(5s);

        int32_t exit_code = std::system(config::api::deploy_command.c_str());

        if (exit_code == 0)
            return;

        LOG_F(ERROR, "Shiro was unable to invoke the deployment command. Process returned %i.", exit_code);
    });
    deploy_thread.detach();
}
