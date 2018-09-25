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

#ifndef SHIRO_MULTIPART_PARSER_HH
#define SHIRO_MULTIPART_PARSER_HH

#include <string>
#include <map>

namespace shiro::utils {

    constexpr uint32_t MULTIPART_TYPE_NONE = 0;
    constexpr uint32_t MULTIPART_TYPE_TEXT = 1;
    constexpr uint32_t MULTIPART_TYPE_FILE = 2;

    class multipart_field {
    public:
        std::string filename;
        std::string content;
        std::string content_type;
        uint32_t multipart_type = MULTIPART_TYPE_NONE;
    };

    using multipart_fields = std::map<std::string, multipart_field>;

    class multipart_parser {
    private:
        std::string body;
        std::string content_type;

    public:
        multipart_parser(const std::string &body, const std::string &content_type);
        ~multipart_parser() = default;

        multipart_fields parse();

    };

    std::string get_field(std::string field_name, const std::string &body);

}

#endif //SHIRO_MULTIPART_PARSER_HH
