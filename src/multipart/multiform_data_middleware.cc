#include <regex>

#include "multiform_data_middleware.hh"
#include "multipart_file.hh"

void shiro::multipart::multiform_data_middleware::before_handle(crow::request &req, crow::response &res, shiro::multipart::multiform_data_middleware::context &ctx) {
    if (req.method != crow::HTTPMethod::Post)
        return;

    const std::string &content_type = req.get_header_value("Content-Type");

    if (content_type.empty() || content_type.find("multipart/form-data") == std::string::npos)
        return;

    static std::regex content_type_regex("Content-Type: (.*?)", std::regex_constants::icase);
    static std::regex content_disposition_regex("Content-Disposition: form-data; name=\"(.*?)\"(?:; filename=\"(.*?)\")?", std::regex_constants::icase);

    static std::string crlf = "\r\n";

    unsigned long boundary_pos = content_type.find("boundary=");

    if (boundary_pos == std::string::npos)
        return;

    std::string boundary = content_type.substr(boundary_pos + 9);
    std::string dash_boundary = "---" + boundary;

    unsigned long pos = req.body.find(dash_boundary);
    if (pos != 0)
        return;

    pos += dash_boundary.size();

    unsigned long next_pos = req.body.find(crlf, pos);

    if (next_pos == std::string::npos)
        return;

    multipart_files files;

    pos += next_pos + crlf.size();

    while (pos < req.body.size()) {
        next_pos = req.body.find(crlf, pos);

        if (next_pos == std::string::npos)
            return;

        std::string name;
        multipart_file file;

        std::string header = req.body.substr(pos, next_pos - pos);

        while (pos != next_pos) {
            std::smatch match;

            if (std::regex_match(header, match, content_type_regex)) {
                file.content_type = match[1];
            } else if (std::regex_match(header, match, content_disposition_regex)) {
                name = match[1];
                file.filename = match[2];
            }

            pos = next_pos + crlf.size();
            next_pos = req.body.find(crlf, pos);

            if (next_pos == std::string::npos)
                return;

            header = req.body.substr(pos, next_pos - pos);
        }

        pos = next_pos + crlf.size();
        next_pos = req.body.find(crlf + dash_boundary);

        if (next_pos == std::string::npos)
            return;

        file.offset = pos;
        file.length = next_pos - pos;

        pos = next_pos + crlf.size() + dash_boundary.size();
        next_pos = req.body.find(crlf, pos);

        if (next_pos == std::string::npos)
            return;

        files.emplace(name, file);

        pos = next_pos + crlf.size();
    }
}

void shiro::multipart::multiform_data_middleware::after_handle(crow::request &req, crow::response &res, shiro::multipart::multiform_data_middleware::context &ctx) {

}
