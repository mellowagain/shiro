/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018-2019 Marc3842h, czapek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdio>
#include <regex>

#include "../logger/sentry_logger.hh"
#include "../thirdparty/loguru.hh"
#include "../utils/string_utils.hh"
#include "beatmap_helper.hh"
#include "beatmap_parser.hh"

shiro::beatmaps::parser::parser(int32_t beatmap_id) : beatmap_id(beatmap_id) {
    // Initialized in initializer list
}

bool shiro::beatmaps::parser::parse() {
    std::shared_ptr<std::ifstream> file = helper::download(this->beatmap_id);

    if (file == nullptr) {
        LOG_F(ERROR, "Unable to parse beatmap %i because it failed to download.", this->beatmap_id);
        return false;
    }

    std::string line;
    std::string current_section;

    while (std::getline(*file, line)) {
        // Skip empty lines
        if (line.empty())
            continue;

        // Comments
        if (boost::algorithm::starts_with(line, " ") || // Lazer
            boost::algorithm::starts_with(line, "_") || // Lazer
            boost::algorithm::starts_with(line, "//")) // Stable
            continue;

        // Skip empty lines after trimming of spaces at the beginning and end
        // This is a separate check so we don't conflict with comments from osu!lazer
        if (boost::algorithm::trim_copy(line).empty())
            continue;

        // osu! file format
        if (line.find("file format v") != std::string::npos) {
            std::string file_format_version = std::regex_replace(line, std::regex(R"([\D])"), "");

            try {
                this->version = boost::lexical_cast<int32_t>(file_format_version);
            } catch (const boost::bad_lexical_cast &ex) {
                LOG_F(WARNING, "Unable to parse file format version for beatmap id %i. (%s -> %s)", this->beatmap_id, line.c_str(), file_format_version.c_str());
                logging::sentry::exception(ex);
            }

            continue;
        }

        // Section header
        if (boost::algorithm::starts_with(line, "[")) {
            current_section = line.substr(1, line.length() - 1);
            continue;
        }

        if (current_section.empty())
            continue;

        // Parse current line in it's section format
        if (current_section == "Metadata") {
            this->parse_metadata(line);
        } else if (current_section == "General") {
            this->parse_general(line);
        } else if (current_section == "Difficulty") {
            this->parse_difficulty(line);
        } else if (current_section == "TimingPoints") {
            this->parse_timing_points(line);
        } else if (current_section == "HitObjects") {
            this->parse_hit_objects(line);
        }
    }

    // Some older maps don't have ar specified, so let's just use od
    if (this->tick_rate == -1.0f)
        this->tick_rate = this->od;

    return true;
}

void shiro::beatmaps::parser::parse_metadata(std::string &line) {
    // This currently splits even when the song title is something like "Re:Re"
    // So in the end it would be: Key:Value:Value2
    // We don't handle that correctly at the moment. Needs fixing in the future when actually used
    std::vector<std::string> parts;
    boost::split(parts, line, boost::is_any_of(":"));

    if (parts.size() < 2)
        return;

    for (std::string &part : parts) {
        boost::algorithm::trim(part);
    }

    std::string key = parts.at(0);
    std::string value = parts.at(1);

    if (key == "Title") {
        this->title = value;
    } else if (key == "TitleUnicode") {
        this->title_unicode = value;
    } else if (key == "Artist") {
        this->artist = value;
    } else if (key == "ArtistUnicode") {
        this->artist_unicode = value;
    } else if (key == "Creator") {
        this->creator = value;
    } else if (key == "Version") {
        this->difficulty_name = value;
    } else if (key == "BeatmapID") {
        int32_t parsed_beatmap_id = 0;

        try {
            parsed_beatmap_id = boost::lexical_cast<int32_t>(value);
        } catch (const boost::bad_lexical_cast &ex) {
            LOG_F(WARNING, "Unable to parse beatmap id from file for beatmap id %i (%s)", this->beatmap_id, value.c_str());
            logging::sentry::exception(ex);
            return;
        }

        if (this->beatmap_id == parsed_beatmap_id)
            return;

        LOG_F(WARNING, "Mismatching beatmap id found while parsing: %i != %i", this->beatmap_id, parsed_beatmap_id);
    }
}

void shiro::beatmaps::parser::parse_general(std::string &line) {
    // Same problem as above
    std::vector<std::string> parts;
    boost::split(parts, line, boost::is_any_of(":"));

    if (parts.size() < 2)
        return;

    for (std::string &part : parts) {
        boost::algorithm::trim(part);
    }

    std::string key = parts.at(0);
    std::string value = parts.at(1);

    if (key == "Mode") {
        try {
            this->mode = (utils::play_mode) boost::lexical_cast<int32_t>(value);
        } catch (const boost::bad_lexical_cast &ex) {
            LOG_F(WARNING, "Unable to parse %s into valid game mode.", value.c_str());
            logging::sentry::exception(ex);
            return;
        }
    }
}

void shiro::beatmaps::parser::parse_difficulty(std::string &line) {
    // Same problem as above
    std::vector<std::string> parts;
    boost::split(parts, line, boost::is_any_of(":"));

    if (parts.size() < 2)
        return;

    for (std::string &part : parts) {
        boost::algorithm::trim(part);
    }

    std::string key = parts.at(0);
    std::string value = parts.at(1);

    if (key == "OverallDifficulty") {
        try {
            this->od = boost::lexical_cast<float>(value);
        } catch (const boost::bad_lexical_cast &ex) {
            LOG_F(WARNING, "Unable to parse %s into float.", value.c_str());
            logging::sentry::exception(ex);
            return;
        }
    } else if (key == "ApproachRate") {
        try {
            this->ar = boost::lexical_cast<float>(value);
        } catch (const boost::bad_lexical_cast &ex) {
            LOG_F(WARNING, "Unable to parse %s into float.", value.c_str());
            logging::sentry::exception(ex);
            return;
        }
    } else if (key == "HPDrainRate") {
        try {
            this->hp = boost::lexical_cast<float>(value);
        } catch (const boost::bad_lexical_cast &ex) {
            LOG_F(WARNING, "Unable to parse %s into float.", value.c_str());
            logging::sentry::exception(ex);
            return;
        }
    } else if (key == "SliderMultiplier") {
        try {
            this->slider_velocity = boost::lexical_cast<float>(value);
        } catch (const boost::bad_lexical_cast &ex) {
            LOG_F(WARNING, "Unable to parse %s into float.", value.c_str());
            logging::sentry::exception(ex);
            return;
        }
    } else if (key == "SliderTickRate") {
        try {
            this->tick_rate = boost::lexical_cast<float>(value);
        } catch (const boost::bad_lexical_cast &ex) {
            LOG_F(WARNING, "Unable to parse %s into float.", value.c_str());
            logging::sentry::exception(ex);
            return;
        }
    }

}

void shiro::beatmaps::parser::parse_timing_points(std::string &line) {
    // Same problem as above
    std::vector<std::string> parts;
    boost::split(parts, line, boost::is_any_of(","));

    if (parts.size() < 2)
        return;

    for (std::string &part : parts) {
        boost::algorithm::trim(part);
    }

    timing_point point;

    try {
        point.time = boost::lexical_cast<double>(parts.at(0));
        point.ms_per_beat = boost::lexical_cast<double>(parts.at(1));
    } catch (const boost::bad_lexical_cast &ex) {
        size_t position = this->timing_points.size() + 1; // std::vector is 0 indexed
        const char *suffix = position == 1 ? "st" : position == 2 ? "nd" : position == 3 ? "rd" : "th";

        LOG_F(WARNING, "Unable to parse %lu%s timing point.", position, suffix);
        logging::sentry::exception(ex);
        return;
    }

    if (parts.size() >= 7)
        point.change = utils::strings::to_bool(parts.at(6));

    this->timing_points.emplace_back(point);
}

void shiro::beatmaps::parser::parse_hit_objects(std::string &line) {
    // Same problem as above
    std::vector<std::string> parts;
    boost::split(parts, line, boost::is_any_of(","));

    if (parts.size() < 4)
        return;

    for (std::string &part : parts) {
        boost::algorithm::trim(part);
    }

    hit_object object;
    double x;
    double y;

    try {
        x = boost::lexical_cast<double>(parts.at(0));
        y = boost::lexical_cast<double>(parts.at(1));
        object.time = boost::lexical_cast<double>(parts.at(2));
        object.type = boost::lexical_cast<int32_t>(parts.at(3));
    } catch (const boost::bad_lexical_cast &ex) {
        size_t position = this->objects.size() + 1; // std::vector is 0 indexed
        const char *suffix = position == 1 ? "st" : position == 2 ? "nd" : position == 3 ? "rd" : "th";

        LOG_F(WARNING, "Unable to parse %lu%s hit object.", position, suffix);
        logging::sentry::exception(ex);
        return;
    }

    object.position = { x, y };

    if (object.type & circle_type) {
        this->circles++;
    } else if (object.type & spinner_type) {
        this->spinners++;
    } else if (object.type & slider_type) {
        if (parts.size() < 8)
            return;

        this->sliders++;

        slider slider;

        // Copy data over from object
        slider.position = object.position;
        slider.time = object.time;
        slider.type = object.type;

        try {
            slider.repetitions = boost::lexical_cast<int32_t>(parts.at(6));
            slider.distance = boost::lexical_cast<double>(parts.at(7));
        } catch (const boost::bad_lexical_cast &ex) {
            size_t position = this->objects.size() + 1; // std::vector is 0 indexed
            const char *suffix = position == 1 ? "st" : position == 2 ? "nd" : position == 3 ? "rd" : "th";

            LOG_F(WARNING, "Unable to parse %lu%s hit object as slider.", position, suffix);
            logging::sentry::exception(ex);
            return;
        }

        this->objects.emplace_back(slider);
        return;
    }

    this->objects.emplace_back(object);
}
