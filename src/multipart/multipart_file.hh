#ifndef SHIRO_MULTIPART_FILE_HH
#define SHIRO_MULTIPART_FILE_HH

#include <map>
#include <string>

namespace shiro::multipart {

    struct multipart_file {
        std::string filename;
        std::string content_type;
        size_t offset = 0;
        size_t length = 0;
    };

    using multipart_files = std::multimap<std::string, multipart_file>;

}

#endif  // SHIRO_MULTIPART_FILE_HH
