#if defined(__has_include)
    #if __has_include(<filesystem>)
        #include <filesystem>

        #if defined(_WIN32)
        // Workaround for Windows: https://docs.microsoft.com/en-us/cpp/standard-library/filesystem
        namespace fs = std::experimental::filesystem::v1;
        #else
        namespace fs = std::filesystem;
        #endif
    #else
        #include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #endif
#else
    // Compiler doesn't support C++17 __has_include so we can expect filesystem is in experimental state
    #include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
#endif

#include <fstream>

#include "replay_manager.hh"

static std::string dir = fs::current_path().u8string() + fs::path::preferred_separator + "replays";

void shiro::replays::init() {
    if (!fs::exists(dir))
        fs::create_directories(dir);
}

void shiro::replays::save_replay(const shiro::scores::score &s, std::string replay) {
    std::string filename = dir + fs::path::preferred_separator + std::to_string(s.id) + ".osr";

    if (fs::exists(filename))
        fs::remove(filename);

    std::ofstream stream(filename, std::ios::trunc);
    stream << replay;
    stream.close();
}

bool shiro::replays::has_replay(const shiro::scores::score &s) {
    return fs::exists(dir + fs::path::preferred_separator + std::to_string(s.id) + ".osr");
}
