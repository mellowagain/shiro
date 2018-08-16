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
