#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <utility>

#include "../thirdparty/loguru.hh"
#include "../utils/crypto.hh"
#include "../utils/string_utils.hh"
#include "replay.hh"

std::string shiro::replays::action::to_string() const {
    return std::to_string(this->w) + "|" +
           std::to_string(this->x) + "|" +
           std::to_string(this->y) + "|" +
           std::to_string(this->z) + ",";
}

shiro::replays::replay::replay(shiro::scores::score s, std::string replay)
    : score(std::move(s))
    , raw_replay(std::move(replay)) {
    // Initialized in initializer list
}

void shiro::replays::replay::parse() {
    std::string decompressed = utils::crypto::lzma::decompress(this->raw_replay);

    if (decompressed.empty())
        return;

    std::vector<std::string> parts;
    boost::split(parts, decompressed, boost::is_any_of(","));

    for (const std::string &part : parts) {
        action a;

        std::vector<std::string> pieces;
        boost::split(pieces, part, boost::is_any_of("|"));

        if (pieces.size() < 4)
            continue;

        try {
            a.w = boost::lexical_cast<int64_t>(pieces.at(0));
            a.x = boost::lexical_cast<float>(pieces.at(1));
            a.y = boost::lexical_cast<float>(pieces.at(2));
            a.z = boost::lexical_cast<int32_t>(pieces.at(3));
        } catch (const boost::bad_lexical_cast &ex) {
            LOG_S(ERROR) << "Unable to cast action values into correct data types:" << ex.what();
            continue;
        }

        this->actions.emplace_back(a);
    }
}

std::vector<shiro::replays::action> shiro::replays::replay::get_actions() {
    return this->actions;
}

std::string shiro::replays::replay::to_string() const {
    std::stringstream stream;

    for (const action &a : this->actions) {
        stream << a.to_string();
    }

    return stream.str();
}
