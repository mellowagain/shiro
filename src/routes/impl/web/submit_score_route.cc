#include "submit_score_route.hh"
#include "../../../thirdparty/loguru.hh"
#include "../../../thirdparty/mpfd/parser.hh"

void shiro::routes::web::submit_score::handle(const crow::request &request, crow::response &response) {
    LOG_F(INFO, "Score has been submitted.");

    std::string content_type = request.get_header_value("content-type");

    if (content_type.empty()) {
        LOG_S(ERROR) << "Received score submission POST without content type.";
        response.code = 400;
        response.end();
        return;
    }

    std::map<std::string, MPFD::Field*> fields;

    try {
        std::unique_ptr<MPFD::Parser> parser = std::make_unique<MPFD::Parser>();

        parser->SetUploadedFilesStorage(MPFD::Parser::StoreUploadedFilesInMemory);
        parser->SetMaxCollectedDataLength(256 * 1024);

        parser->SetContentType(content_type);
        parser->AcceptSomeData(request.body.c_str(), request.body.length());

        fields = parser->GetFieldsMap();
    } catch (MPFD::Exception &ex) {
        LOG_S(ERROR) << "Unable to parse multipart data: " << ex.GetError() << ".";
        response.code = 500;
        response.end();
        return;
    }

    if (fields.empty()) {
        LOG_S(ERROR) << "Received score submission with empty fields.";
        response.code = 400;
        response.end();
        return;
    }

    for (auto &key_value : fields) {
        if (key_value.second->GetType() == 2) {
            LOG_S(INFO) << "File Field: " << key_value.first << " - Name: " << key_value.second->GetFileName() << " - MIME: " << key_value.second->GetFileMimeType();
        } else {
            LOG_S(INFO) << "Text Field: " << key_value.first << " - Value: " << key_value.second->GetTextTypeContent();
        }

    }
}
