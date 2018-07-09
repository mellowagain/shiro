#include "../thirdparty/cpptoml.hh"
#include "../thirdparty/loguru.hh"
#include "cli_args.hh"
#include "db_file.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

std::string shiro::config::database::address = "127.0.0.1";
unsigned int shiro::config::database::port = 3306;
std::string shiro::config::database::database = "shiro";
std::string shiro::config::database::username = "root";
std::string shiro::config::database::password = "hunter2";

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

    address = config_file->get_qualified_as<std::string>("database.address").value_or("127.0.0.1");
    port = config_file->get_qualified_as<unsigned int>("database.port").value_or(3306);
    database = config_file->get_qualified_as<std::string>("database.database").value_or("shiro");
    username = config_file->get_qualified_as<std::string>("database.username").value_or("root");
    password = config_file->get_qualified_as<std::string>("database.password").value_or("hunter2");

    LOG_S(INFO) << "Successfully parsed database.toml.";

    cli::cli_app.add_option("--db-address", address, "Address of MySQL server to connect to");
    cli::cli_app.add_option("--db-port", port, "Port of MySQL server to connect to");
    cli::cli_app.add_option("--db-database", database, "Database in MySQL server to put data into");
    cli::cli_app.add_option("--db-username", username, "Username used to authenticate with MySQL server");
    cli::cli_app.add_option("--db-password", password, "Password used to authenticate with MySQL server");
}
