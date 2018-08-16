#include "../../../thirdparty/loguru.hh"
#include "../../../utils/multipart_parser.hh"
#include "submit_score_route.hh"

void shiro::routes::web::submit_score::handle(const crow::request &request, crow::response &response) {
    LOG_F(INFO, "Score has been submitted.");

    std::string content_type = request.get_header_value("content-type");

    if (content_type.empty()) {
        LOG_S(ERROR) << "Received score submission POST without content type.";
        response.code = 400;
        response.end();
        return;
    }

    std::unique_ptr<utils::multipart_parser> parser = std::make_unique<utils::multipart_parser>(request.body, content_type);

    if (parser == nullptr)
        return;

    utils::multipart_fields fields = parser->parse();
}
