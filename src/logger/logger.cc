// Loguro settings
#if defined(_DEBUG)
    #define LOGURU_DEBUG_LOGGING 1
    #define LOGURU_DEBUG_CHECKS 1
#else
    #undef LOGURU_DEBUG_LOGGING
    #undef LOGURU_DEBUG_CHECKS
#endif

#define LOGURU_IMPLEMENTATION 1

#include "../thirdparty/loguru.hh"
#include "logger.hh"

void shiro::logging::init(int argc, char **argv) {
    loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
    loguru::g_flush_interval_ms = 100;

    std::string current_time = []() -> std::string {
        struct std::tm time_struct;
        char buffer[80];

        std::time_t now = std::time(nullptr);
        time_struct = *std::localtime(&now);

        std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H.%M.%S", &time_struct);

        return std::string(buffer);
    }();
    char buffer[128];
    std::snprintf(buffer, sizeof(buffer), "logs/shiro %s.log", current_time.c_str());
    loguru::add_file(buffer, loguru::FileMode::Append, loguru::Verbosity_INFO);

    loguru::init(argc, argv);

    LOG_S(INFO) << "Logging was successfully initialized.";
}
