#include <algorithm>

#include "escaper.hh"

std::string shiro::utils::escaper::make_safe(std::string input) {
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);

    input.erase(std::remove_if(input.begin(), input.end(), [](char c) {
        return !std::isalnum(c) && !std::isspace(c);
    }), input.end());

    std::replace(input.begin(), input.end(), ' ', '_');
    return input;
}
