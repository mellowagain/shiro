#include "../thirdparty/loguru.hh"
#include "cli_args.hh"
#include "db_file.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

void shiro::config::database::parse() {
    // For convenience
    using namespace cpptoml;

    if (config_file != nullptr)
        LOG_S(INFO) << "Re-parsing database.toml file...";

    try {
        config_file = parse_file("database.toml");
    } catch (const parse_exception &ex) {
        LOG_S(FATAL) << "Failed to parse database.toml file: " << ex.what() << ".";
    }

    address = *config_file->get_as<std::string>("database.address");
    port = *config_file->get_as<unsigned int>("database.port");
    database = *config_file->get_as<std::string>("database.database");
    username = *config_file->get_as<std::string>("database.username");
    password = *config_file->get_as<std::string>("database.password");

    LOG_S(INFO) << "Successfully parsed database.toml.";

    cli::cli_app.add_option("--db-address", address, "Address of MySQL server to connect to")->required(false);
    cli::cli_app.add_option("--db-port", port, "Port of MySQL server to connect to")->required(false);
    cli::cli_app.add_option("--db-database", database, "Database in MySQL server to put data into")->required(false);
    cli::cli_app.add_option("--db-username", username, "Username used to authenticate with MySQL server")->required(false);
    cli::cli_app.add_option("--db-password", password, "Password used to authenticate with MySQL server")->required(false);
}
