#include "../thirdparty/cpptoml.hh"
#include "../thirdparty/loguru.hh"
#include "bancho_file.hh"
#include "cli_args.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

std::string shiro::config::bancho::host = "localhost";
int shiro::config::bancho::port = 8080;

void shiro::config::bancho::parse() {
    // For convenience
    using namespace cpptoml;

    if (config_file != nullptr)
        LOG_S(INFO) << "Re-parsing bancho.toml file...";

    try {
        config_file = parse_file("bancho.toml");
    } catch (const parse_exception &ex) {
        LOG_S(FATAL) << "Failed to parse bancho.toml file: " << ex.what() << ".";
    }

    host = config_file->get_qualified_as<std::string>("server.host").value_or("localhost");
    port = config_file->get_qualified_as<int>("server.port").value_or(8080);

    LOG_S(INFO) << "Successfully parsed bancho.toml.";

    cli::cli_app.add_option("--bancho-host", host, "Host that Bancho should bind to");
    cli::cli_app.add_option("--bancho-port", port, "Port that Bancho should bind to");
}
