#include "../../config/direct_file.hh"
#include "../../utils/curler.hh"
#include "emulation.hh"

std::tuple<bool, std::string> shiro::direct::emulation::search(std::unordered_map<std::string, std::string> parameters) {
    // Remove username from the request so the requesting user stays anonymous
    if (parameters.find("u") != parameters.end())
        parameters.erase("u");

    // Remove password hash from the request so no credentials are leaked
    if (parameters.find("h") != parameters.end())
        parameters.erase("h");

    std::string url = config::direct::base_url + "/web/osu-search.php?";

    for (const auto &[key, value] : parameters) {
        url.append(key).append("=").append(value).append("&");
    }

    // Remove the last char (which will be a &)
    url = url.substr(0, url.length() - 1);

    return utils::curl::get_direct(url);
}

std::tuple<bool, std::string> shiro::direct::emulation::download(int32_t beatmap_id, bool no_video) {
    std::string url = config::direct::mirror_url + "/d/" + std::to_string(beatmap_id);

    if (no_video)
        url.append("?novideo=yes");

    return utils::curl::get_direct(url);
}

const std::string shiro::direct::emulation::name() const {
    return "Emulation";
}
