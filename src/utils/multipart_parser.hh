/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2020 Marc3842h, czapek
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

#ifndef SHIRO_MULTIPART_PARSER_HH
#define SHIRO_MULTIPART_PARSER_HH

#include <string>
#include <map>
#include <vector>

namespace shiro::utils {

    enum class multipart_field_type {
        none = 0,
        text = 1,
        file = 2
    };

    class multipart_form_part {
    public:
        std::map<std::string, std::string> headers;
        std::string body;
    };

    class multipart_form_field {
    public:
        std::string name;
        std::string content_type;
        std::string body;
        multipart_field_type type = multipart_field_type::none;
    };

    using multipart_form_parts = std::vector<multipart_form_part>;
    using multipart_form_fields = std::map<std::string, multipart_form_field>;

    class multipart_parser {
    private:
        std::string body;
        std::string content_type;

    public:
        multipart_parser(const std::string &body, const std::string &content_type);
        ~multipart_parser() = default;

        multipart_form_parts parse();
        multipart_form_fields parse_fields();

    };

}

#endif //SHIRO_MULTIPART_PARSER_HH
