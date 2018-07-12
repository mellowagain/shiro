#include "../thirdparty/loguru.hh"
#include "route_logger.hh"

void shiro::logging::route_logger::log(std::string message, crow::LogLevel level) {
    switch (level) {
        case crow::LogLevel::INFO:
            LOG_S(INFO) << message;
            break;
        case crow::LogLevel::WARNING:
            LOG_S(WARNING) << message;
            break;
        case crow::LogLevel::ERROR:
            LOG_S(ERROR) << message;
            break;
        case crow::LogLevel::CRITICAL:
            LOG_S(FATAL) << message;
            break;
        default:
            // Do nothing
            break;
    }
}
