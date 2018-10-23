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

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <regex>

#include "../thirdparty/multipartparser.hh"
#include "multipart_parser.hh"
#include "string_utils.hh"

shiro::utils::multipart_parser::multipart_parser(const std::string &body, const std::string &content_type)
    : body(body)
    , content_type(content_type) {
    // Initialized in initializer list
}

shiro::utils::multipart_form_parts shiro::utils::multipart_parser::parse() {
    if (this->content_type.find("multipart/form-data") == std::string::npos)
        return {};

    std::string::size_type boundary_pos = this->content_type.find("boundary=");

    if (boundary_pos == std::string::npos)
        return {};

    std::string boundary = this->content_type.substr(boundary_pos + 9);

    bool body_begin_called = false;
    bool body_end_called = false;

    std::string header_name;
    std::string header_value;

    multipart_form_parts parts {};

    const auto on_body_begin = [&](multipartparser *parser) -> int {
        body_begin_called = true;
        return 0;
    };
    const auto on_part_begin = [&](multipartparser *parser) -> int {
        parts.emplace_back(multipart_form_part {});
        return 0;
    };
    const auto on_header_done = [&]() -> int {
        parts.back().headers[header_name] = header_value;

        header_name.clear();
        header_value.clear();

        return 0;
    };
    const auto on_header_field = [&](multipartparser *parser, const char *data, size_t size) -> int {
        if (!header_value.empty())
            on_header_done();

        header_name.append(data, size);
        return 0;
    };
    const auto on_header_value = [&](multipartparser *parser, const char *data, size_t size) -> int {
        header_value.append(data, size);
        return 0;
    };
    const auto on_headers_complete = [&](multipartparser *parser) -> int {
        if (!header_value.empty())
            on_header_done();

        return 0;
    };
    const auto on_data = [&](multipartparser *parser, const char *data, size_t size) -> int {
        parts.back().body.append(data, size);
        return 0;
    };
    const auto on_part_end = [](multipartparser *parser) -> int {
        return 0;
    };
    const auto on_body_end = [&](multipartparser *parser) -> int {
        body_end_called = true;
        return 0;
    };

    multipartparser parser {};
    multipartparser_callbacks callbacks {};

    multipartparser_callbacks_init(&callbacks);

    callbacks.on_body_begin = on_body_begin;
    callbacks.on_part_begin = on_part_begin;
    callbacks.on_header_field = on_header_field;
    callbacks.on_header_value = on_header_value;
    callbacks.on_headers_complete = on_headers_complete;
    callbacks.on_data = on_data;
    callbacks.on_part_end = on_part_end;
    callbacks.on_body_end = on_body_end;

    multipartparser_init(&parser, boundary.c_str());
    size_t result = multipartparser_execute(&parser, &callbacks, this->body.c_str(), this->body.size());

    if (result != this->body.size())
        return {};

    return parts;
}

shiro::utils::multipart_form_fields shiro::utils::multipart_parser::parse_fields() {
    multipart_form_parts parts = this->parse();
    multipart_form_fields fields {};

    for (const multipart_form_part &part : parts) {
        if (part.headers.find("Content-Disposition") == part.headers.end())
            continue;

        static const std::regex field_name_regex(".+name=\"(.+)\"");
        static const std::regex field_filename_regex(".+filename=\"(.+)\"");

        std::smatch field_name_regex_match, field_filename_regex_match;

        if (part.headers.find("Content-Type") != part.headers.end()) {
            if (std::regex_match(part.headers.at("Content-Disposition"), field_filename_regex_match, field_filename_regex)) {
                std::string field_name = field_filename_regex_match[1];
                multipart_form_field field;

                // workaround for peppy's gay
                if (field_filename_regex_match[1] == "score")
                    field_name = "replay-bin";

                field.body = part.body;
                field.content_type = part.headers.at("Content-Type");
                field.name = field_name;
                field.type = multipart_field_type::file;

                fields.insert({ field_name, field });
            }
        } else {
            if (std::regex_match(part.headers.at("Content-Disposition"), field_name_regex_match, field_name_regex)) {
                multipart_form_field field;

                field.body = part.body;
                field.content_type = "text/plain";
                field.name = field_name_regex_match[1];
                field.type = multipart_field_type::text;

                fields.insert({ field_name_regex_match[1], field });
            }
        }
    }

    return fields;
}
