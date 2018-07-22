#include "../thirdparty/cpptoml.hh"
#include "../thirdparty/loguru.hh"
#include "bot_file.hh"
#include "cli_args.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

std::string shiro::config::bot::name = "Shiro";

void shiro::config::bot::parse() {
    // For convenience
    using namespace cpptoml;

    if (config_file != nullptr)
        LOG_S(INFO) << "Re-parsing bot.toml file...";

    try {
        config_file = parse_file("bot.toml");
    } catch (const parse_exception &ex) {
        LOG_S(FATAL) << "Failed to parse bot.toml file: " << ex.what() << ".";
    }

    name = config_file->get_qualified_as<std::string>("bot.name").value_or("Shiro");

    LOG_S(INFO) << "Successfully parsed bot.toml.";

    cli::cli_app.add_option("--bot-name", name, "Name of the chat bot");
}
