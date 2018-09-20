#include <cstdlib>
#include <curl/curl.h>
#include <thread>

#include "bot/bot.hh"
#include "channels/channel_manager.hh"
#include "config/bancho_file.hh"
#include "config/bot_file.hh"
#include "config/cli_args.hh"
#include "config/db_file.hh"
#include "config/score_submission_file.hh"
#include "geoloc/country_ids.hh"
#include "logger/logger.hh"
#include "native/signal_handler.hh"
#include "native/system_statistics.hh"
#include "replays/replay_manager.hh"
#include "routes/routes.hh"
#include "thirdparty/cli11.hh"
#include "thirdparty/loguru.hh"
#include "users/user_activity.hh"
#include "users/user_timeout.hh"
#include "shiro.hh"

std::shared_ptr<shiro::database> shiro::db_connection = nullptr;
tsc::TaskScheduler shiro::scheduler;
std::time_t shiro::start_time = std::time(nullptr);

int shiro::init(int argc, char **argv) {
    logging::init(argc, argv);

    curl_global_init(CURL_GLOBAL_DEFAULT);

    config::cli::parse(argc, argv);

    config::bot::parse();
    config::bancho::parse();
    config::database::parse();
    config::score_submission::parse();

    geoloc::init();

    db_connection = std::make_shared<database>(
            config::database::address, config::database::port, config::database::database,
            config::database::username, config::database::password
    );
    db_connection->connect();
    db_connection->setup();

    std::thread scheduler_updater([&]() {
        while (true) {
            std::this_thread::sleep_for(1ms);
            scheduler.Update(1ms);
        }
    });
    scheduler_updater.detach(); // The root of all suffering is attachment

    bot::init();
    bot::init_commands();

    channels::manager::init();

    users::activity::init();
    users::timeout::init();

    replays::init();

    native::system_stats::init();
    native::signal_handler::install();

    LOG_F(INFO, "Welcome to shiro. Listening on http://%s:%i/.", config::bancho::host.c_str(), config::bancho::port);
    LOG_F(INFO, "Press CTRL + C to quit.");

    routes::init();

    return EXIT_SUCCESS;
}

void shiro::destroy() {
    curl_global_cleanup();

    scheduler.CancelAll();

    LOG_S(INFO) << "Thank you and goodbye.";
}
