#include "../thirdparty/loguru.hh"
#include "route_logger.hh"

void shiro::logging::route_logger::log(std::string message, crow::LogLevel level) {
    switch (level) {
        case crow::LogLevel::Info:
            LOG_S(INFO) << message;
            break;
        case crow::LogLevel::Warning:
            LOG_S(WARNING) << message;
            break;
        case crow::LogLevel::Error:
            LOG_S(ERROR) << message;
            break;
        case crow::LogLevel::Critical:
            LOG_S(FATAL) << message;
            break;
        default:
            // Do nothing
            break;
    }
}
