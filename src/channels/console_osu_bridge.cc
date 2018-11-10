#include "../utils/bot_utils.hh"
#include "console_osu_bridge.hh"

void shiro::channels::bridge::install() {
    loguru::add_callback("osu! bridge", channels::bridge::callback, nullptr, loguru::Verbosity_INFO);
}

void shiro::channels::bridge::callback(void *user_data, const loguru::Message &message) {
    utils::bot::respond(message.message, nullptr, "#console");
}
