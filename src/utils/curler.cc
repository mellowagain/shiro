/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018 Marc3842h, czapek
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

#include "curler.hh"

#include <curl/curl.h>
#include <fstream>

#include "../thirdparty/loguru.hh"

#include "../config/curl_file.hh"

shiro::utils::curler::curler() {
    c = curl_easy_init();

    curl_easy_setopt(c, CURLOPT_SSL_VERIFYPEER, shiro::config::curl::cert_verify);
    curl_easy_setopt(c, CURLOPT_USERAGENT, shiro::config::curl::user_agent.c_str());
}

bool shiro::utils::curler::valid() {
    return c != nullptr;
}

shiro::utils::curler::~curler() {
    curl_easy_cleanup(c);
}

void shiro::utils::curler::set_callback(void *callback) {
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, callback);
}

void shiro::utils::curler::set_output(void *output) {
    curl_easy_setopt(c, CURLOPT_WRITEDATA, output);
}

void shiro::utils::curler::set_user_agent(const std::string &user_agent) {
    curl_easy_setopt(c, CURLOPT_USERAGENT, user_agent.c_str());
}

CURLcode shiro::utils::curler::perform(const std::string &url) {
    curl_easy_setopt(c, CURLOPT_URL, url.c_str());
    return curl_easy_perform(c);
}
