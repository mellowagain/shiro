#include <boost/lexical_cast.hpp>
#include "string_utils.hh"

std::vector<std::string> shiro::utils::strings::split(const std::string &input, char separator) {
    std::vector<std::string> result;

    std::string::size_type prev_pos = 0;
    std::string::size_type pos = 0;

    while ((pos = input.find(separator, pos)) != std::string::npos) {
        std::string sub_string = input.substr(prev_pos, pos - prev_pos);

        result.emplace_back(sub_string);
        prev_pos = ++pos;
    }

    // Last part of string
    result.emplace_back(input.substr(prev_pos, pos - prev_pos));

    return result;
}

bool shiro::utils::strings::to_bool(std::string src) {
    try {
        return boost::lexical_cast<bool>(src);
    } catch (const boost::bad_lexical_cast &ex) {
        std::transform(src.begin(), src.end(), src.begin(), ::tolower);
        return src == "true";
    }
}
