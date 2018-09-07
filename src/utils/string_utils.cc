#include <boost/lexical_cast.hpp>
#include "string_utils.hh"

bool shiro::utils::strings::to_bool(std::string src) {
    try {
        return boost::lexical_cast<bool>(src);
    } catch (const boost::bad_lexical_cast &ex) {
        std::transform(src.begin(), src.end(), src.begin(), ::tolower);
        return src == "true";
    }
}
