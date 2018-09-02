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

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/lzma.hpp> // Segfaults: https://stackoverflow.com/questions/50071513/c-boost-and-lzma-decompression
#include <boost/iostreams/copy.hpp>
#include <fstream>
#include <sstream>
#include <string>

#include "../io/osu_buffer.hh"
#include "../thirdparty/loguru.hh"
#include "../users/user_manager.hh"
#include "../utils/crypto.hh"
#include "../utils/osu_string.hh"
#include "../utils/time_utils.hh"
#include "replay.hh"
#include "replay_manager.hh"

static std::string dir = fs::current_path().u8string() + fs::path::preferred_separator + "replays";

void shiro::replays::init() {
    if (!fs::exists(dir))
        fs::create_directories(dir);
}

void shiro::replays::save_replay(const shiro::scores::score &s, int32_t game_version, std::string replay) {
    std::string filename = dir + fs::path::preferred_separator + std::to_string(s.id) + ".osr";

    std::shared_ptr<users::user> user = users::manager::get_user_by_id(s.user_id);

    if (user == nullptr)
        return;

    if (fs::exists(filename))
        fs::remove(filename);

    // Convert raw replay into full osu! replay file
    // Reference: https://osu.ppy.sh/help/wiki/osu!_File_Formats/Osr_(file_format)

    char hash_buffer[1024];

    // poot are you?
    std::snprintf(hash_buffer, sizeof(hash_buffer), "%ip%io%io%it%ia%sr%ie%sy%so%liu%s%i%s",
            s._100_count + s._300_count, s._50_count, s.gekis_count, s.katus_count, s.miss_count,
            s.beatmap_md5.c_str(), s.max_combo, s.fc ? "True" : "False",
            user->presence.username.c_str(), s.total_score, s.rank.c_str(), s.mods, "True");

    std::string beatmap_md5 = utils::osu_string(s.beatmap_md5);
    std::string username = utils::osu_string(user->presence.username);
    std::string hash = utils::osu_string(utils::crypto::md5::hash(hash_buffer));
    std::string diagram = utils::osu_string(calculate_diagram(s, replay), true);

    io::buffer buffer;

    buffer.write<uint8_t>(s.play_mode);
    buffer.write<int32_t>(game_version);

    buffer.write_string(beatmap_md5);
    buffer.write_string(username);
    buffer.write_string(hash);

    buffer.write<int16_t>(s._300_count);
    buffer.write<int16_t>(s._100_count);
    buffer.write<int16_t>(s._50_count);
    buffer.write<int16_t>(s.gekis_count);
    buffer.write<int16_t>(s.katus_count);
    buffer.write<int16_t>(s.miss_count);

    buffer.write<int32_t>(s.total_score);
    buffer.write<int16_t>(s.max_combo);
    buffer.write<uint8_t>(s.fc);
    buffer.write<int32_t>(s.mods);

    buffer.write_string(diagram);
    buffer.write<int64_t>(utils::time::get_current_time_ticks());

    buffer.write<int32_t>(replay.size());
    buffer.write_string(replay);

    buffer.write<int64_t>(s.id);

    std::ofstream stream(filename, std::ios::trunc);
    stream << buffer.serialize();
    stream.close();

    // If the replay is bigger than 1 mib, compress it
    // TODO: Implement a smarter compression method than zlib
    if (fs::file_size(filename) >= 1048576) {
        fs::remove(filename);

        filename = dir + fs::path::preferred_separator + std::to_string(s.id) + ".osr.zz";

        std::stringstream original;
        std::stringstream compressed;

        original << buffer.serialize();

        boost::iostreams::filtering_streambuf<boost::iostreams::input> output;
        output.push(boost::iostreams::zlib_compressor());
        output.push(original);

        boost::iostreams::copy(output, compressed);

        stream = std::ofstream(filename, std::ios::trunc);
        stream << compressed.str();
        stream.close();

        LOG_S(WARNING) << "Uncompressed replay was >1mb, saved replay with zlib compression.";
    }
}

std::string shiro::replays::calculate_diagram(const shiro::scores::score &s, std::string raw_replay) {
    std::unique_ptr<replays::replay> replay = std::make_unique<replays::replay>(s, raw_replay);

    replay->parse();

    if (replay->get_actions().empty())
        return "";

    std::stringstream stream;
    float last_x = 0.0f;

    for (size_t i = 0; i < replay->get_actions().size(); i++) {
        replays::action action = replay->get_actions().at(i);

        if (((action.x - last_x) > 2000.0f) || (i == (replay->get_actions().size() - 1)) || (i == 0)) {
            last_x = action.x;

            char buffer[32];
            std::snprintf(buffer, sizeof(buffer), "%.2f|%.2f,", action.x, action.y);

            stream << buffer;
        }
    }

    return stream.str();
}

bool shiro::replays::has_replay(const shiro::scores::score &s) {
    if (fs::exists(dir + fs::path::preferred_separator + std::to_string(s.id) + ".osr"))
        return true;

    return fs::exists(dir + fs::path::preferred_separator + std::to_string(s.id) + ".osr.zz");
}
