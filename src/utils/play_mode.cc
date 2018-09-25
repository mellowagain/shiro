#include "play_mode.hh"

std::string shiro::utils::play_mode_to_string(shiro::utils::play_mode mode) {
    switch (mode) {
        case play_mode::taiko:
            return "osu!taiko";
        case play_mode::fruits:
            return "osu!catch";
        case play_mode::mania:
            return "osu!mania";
        case play_mode::standard:
        default:
            return "osu!";
    }
}
