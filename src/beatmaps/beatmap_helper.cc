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

#include <curl/curl.h>
#include <fstream>

#include "../thirdparty/loguru.hh"
#include "beatmap_helper.hh"

static std::string dir = fs::current_path().u8string() + fs::path::preferred_separator + "maps";

void shiro::beatmaps::helper::init() {
    if (!fs::exists(dir))
        fs::create_directories(dir);
}

int32_t shiro::beatmaps::helper::fix_beatmap_status(int32_t status_code) {
    if (status_code == (int32_t) status::needs_update)
        return (int32_t) status::ranked;

    if (status_code == (int32_t) status::qualified)
        return (int32_t) status::loved;

    return status_code;
}

bool shiro::beatmaps::helper::has_leaderboard(int32_t status_code) {
    return status_code == (int32_t) status::ranked ||
           status_code == (int32_t) status::loved ||
           status_code == (int32_t) status::qualified ||
           status_code == (int32_t) status::approved;
}

bool shiro::beatmaps::helper::awards_pp(int32_t status_code) {
    return status_code == (int32_t) status::ranked;
}

size_t shiro::beatmaps::helper::callback(void *raw_data, size_t size, size_t memory, std::string *ptr) {
    size_t new_length = size * memory;
    size_t old_length = ptr->size();

    try {
        ptr->resize(old_length + new_length);
    } catch (const std::bad_alloc &ex) {
        LOG_F(ERROR, "Unable to allocate new memory for callback of geo location service: %s.", ex.what());
        return 0;
    }

    std::copy((char*) raw_data, (char*) raw_data + new_length, ptr->begin() + old_length);
    return size * memory;
}

FILE *shiro::beatmaps::helper::download(int32_t beatmap_id) {
    std::string filename = dir + fs::path::preferred_separator + std::to_string(beatmap_id) + ".osu";

    if (fs::exists(filename))
        return fopen(filename.c_str(), "r");

    CURL *curl = curl_easy_init();
    CURLcode status_code;

    std::string output;

    if (curl != nullptr) {
        char buffer[512];
        std::snprintf(buffer, sizeof(buffer), "https://old.ppy.sh/osu/%i", beatmap_id);

        curl_easy_setopt(curl, CURLOPT_URL, buffer);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64; rv:61.0) Gecko/20100101 Firefox/61.0"); // shiro (https://github.com/Marc3842h/shiro)

        status_code = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (status_code != CURLE_OK) {
            LOG_F(ERROR, "Received invalid response from Bancho API: %s.", curl_easy_strerror(status_code));
            return nullptr;
        }
    }

    std::ofstream stream(filename, std::ios::trunc);
    stream << output;
    stream.close();

    return fopen(filename.c_str(), "r");
}
