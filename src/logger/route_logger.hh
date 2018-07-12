#ifndef SHIRO_ROUTE_LOGGER_HH
#define SHIRO_ROUTE_LOGGER_HH

#include "../thirdparty/crow.hh"

namespace shiro::logging {

    class route_logger : public crow::ILogHandler {
    public:
        void log(std::string message, crow::LogLevel level) override;

    };

}

#endif //SHIRO_ROUTE_LOGGER_HH
