#include "../logger/sentry_logger.hh"
#include "../thirdparty/cpptoml.hh"
#include "../thirdparty/loguru.hh"
#include "cli_args.hh"
#include "ipc_file.hh"

static std::shared_ptr<cpptoml::table> config_file = nullptr;

std::string shiro::config::ipc::backend_url = "https://c.shiro.host/";
std::string shiro::config::ipc::frontend_url = "https://shiro.host/";
std::string shiro::config::ipc::beatmap_url = "https://osu.ppy.sh/b/";

void shiro::config::ipc::parse() {
    if (config_file != nullptr)
        LOG_F(INFO, "Re-parsing ipc.toml file...");

    try {
        config_file = cpptoml::parse_file("ipc.toml");
    } catch (const cpptoml::parse_exception &ex) {
        logging::sentry::exception(ex);
        ABORT_F("Failed to parse ipc.toml file: %s.", ex.what());
    }

    backend_url = config_file->get_qualified_as<std::string>("meta.backend_url").value_or("https://c.shiro.host/");
    frontend_url = config_file->get_qualified_as<std::string>("meta.frontend_url").value_or("https://shiro.host/");
    beatmap_url = config_file->get_qualified_as<std::string>("meta.beatmap_url").value_or("https://osu.ppy.sh/b/");

    LOG_F(INFO, "Successfully parsed ipc.toml.");

    cli::cli_app.add_option("--ipc-backend-url", backend_url, "Url of the Bancho server (this instance)");
    cli::cli_app.add_option("--ipc-frontend-url", frontend_url, "Url of the frontend server");
    cli::cli_app.add_option("--ipc-beatmap-url", beatmap_url, "Url of the beatmap detail page on the frontend");
}
