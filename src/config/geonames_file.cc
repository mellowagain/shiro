#include "../thirdparty/cpptoml.hh"
#include "../thirdparty/loguru.hh"
#include "cli_args.hh"
#include "geonames_file.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

std::string shiro::config::geonames::username = "demo";

void shiro::config::geonames::parse() {
    // For convenience
    using namespace cpptoml;

    if (config_file != nullptr)
        LOG_S(INFO) << "Re-parsing geonames.toml file...";

    try {
        config_file = parse_file("geonames.toml");
    } catch (const parse_exception &ex) {
        LOG_S(FATAL) << "Failed to parse geonames.toml file: " << ex.what() << ".";
    }

    username = config_file->get_qualified_as<std::string>("geonames.username").value_or("demo");

    LOG_S(INFO) << "Successfully parsed geonames.toml.";

    cli::cli_app.add_option("--geonames-username", username, "API username for geonames.org");
}
