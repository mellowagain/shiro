#ifndef SHIRO_DIRECT_PROVIDER_HH
#define SHIRO_DIRECT_PROVIDER_HH

#include <string>
#include <tuple>
#include <memory>
#include <unordered_map>

namespace shiro::direct {

    class direct_provider {
    public:
        virtual std::tuple<bool, std::string> search(std::unordered_map<std::string, std::string> parameters);
        virtual std::tuple<bool, std::string> download(int32_t beatmap_id, bool no_video);

        virtual const std::string name() const;

    };

    extern std::shared_ptr<direct_provider> provider;

    void init();

    // Checks if the current provider is working
    bool sanity_check();

}

#endif //SHIRO_DIRECT_PROVIDER_HH
