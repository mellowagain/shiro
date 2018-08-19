#ifndef SHIRO_STRING_UTILS_HH
#define SHIRO_STRING_UTILS_HH

#include <vector>
#include <string>

namespace shiro::utils::strings {

    std::vector<std::string> split(const std::string &input, char separator);

    bool to_bool(std::string src);

}

#endif //SHIRO_STRING_UTILS_HH
