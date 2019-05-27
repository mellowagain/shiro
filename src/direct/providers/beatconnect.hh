#ifndef SHIRO_BEATCONNECT_HH
#define SHIRO_BEATCONNECT_HH

#include "../direct_provider.hh"

namespace shiro::direct {

    class beatconnect : public direct_provider {
    public:
        std::tuple<bool, std::string> search(std::unordered_map<std::string, std::string> parameters) override;
        std::tuple<bool, std::string> download(int32_t beatmap_id, bool no_video) override;

        const std::string name() const override;

    private:
        // Translates osu!direct arguments to their Beatconnect.io equivalents
        std::tuple<std::string, std::string> sanitize_args(std::string key, std::string value, bool url_escape = false);

        static void sanitize_mode(std::string &value);
        static void sanitize_status(std::string &value);
        static void sanitize_query(std::string &value);

    };

}

#endif //SHIRO_BEATCONNECT_HH
