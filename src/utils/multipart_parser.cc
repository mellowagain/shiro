#include <boost/algorithm/string.hpp>
#include <regex>

#include "multipart_parser.hh"
#include "string_utils.hh"

shiro::utils::multipart_parser::multipart_parser(const std::string &body, const std::string &content_type)
    : body(body)
    , content_type(content_type) {
    // Initialized in initializer list
}

shiro::utils::multipart_fields shiro::utils::multipart_parser::parse() {
    if (this->content_type.find("multipart/form-data") == std::string::npos)
        return {};

    std::string::size_type boundary_pos = this->content_type.find("boundary=");

    if (boundary_pos == std::string::npos)
        return {};

    std::string boundary = this->content_type.substr(boundary_pos + 9);
    std::string dash_boundary = "--" + boundary;

    std::string::size_type prev_pos = 0;
    std::string::size_type pos = 0;

    multipart_fields fields;

    while ((pos = this->body.find(dash_boundary, pos)) != std::string::npos) {
        std::string substr = this->body.substr(prev_pos, pos - prev_pos);
        prev_pos = ++pos;

        if (substr.length() < dash_boundary.length())
            continue;

        if (substr.find("Content-Disposition: form-data;") == std::string::npos)
            continue;

        std::vector<std::string> lines = strings::split(substr, '\r');

        if (lines.size() < 2)
            continue;

        for (std::string &line : lines) {
            boost::replace_all(line, "\n", "");
        }

        multipart_field field;
        field.multipart_type = MULTIPART_TYPE_TEXT;

        std::string name = get_field("name", lines.at(1));
        std::string filename = get_field("filename", lines.at(1));
        std::string content_type;

        if (!filename.empty())
            field.multipart_type = MULTIPART_TYPE_FILE;

        if (lines.size() < (field.multipart_type == MULTIPART_TYPE_TEXT ? 3 : 4))
            continue;

        if (field.multipart_type == MULTIPART_TYPE_FILE) {
            content_type = lines.at(2);
            boost::replace_all(content_type, "Content-Type: ", "");

            if (content_type.empty())
                continue;
        }

        switch (field.multipart_type) {
            case MULTIPART_TYPE_TEXT: {
                field.content = lines.at(3);
                break;
            }
            case MULTIPART_TYPE_FILE: {
                if (name == "score")
                    name = "replay";

                field.filename = filename;
                field.content_type = content_type;

                std::stringstream stream;
                for (size_t i = 4; i < lines.size(); i++) {
                    stream << lines.at(i);
                }

                field.content = stream.str();
                break;
            }
            default: {
                continue;
            }
        }

        fields.insert(std::make_pair(name, field));
    }

    return fields;
}

std::string shiro::utils::get_field(std::string field_name, const std::string &body) {
    field_name = field_name + "=\"";

    std::string::size_type pos = body.find(field_name);

    if (pos == std::string::npos)
        return "";

    std::string substr = body.substr(pos);

    static std::regex regex(R"((["'])(?:(?=(\\?))\2.)*?\1)");
    std::smatch match;

    if (!std::regex_search(substr, match, regex))
        return "";

    if (match.empty())
        return "";

    std::string matched = match.str(0);
    boost::replace_all(matched, "\"", "");

    return matched;
}
