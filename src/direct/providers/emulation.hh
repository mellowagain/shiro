#ifndef SHIRO_EMULATION_HH
#define SHIRO_EMULATION_HH

#include "../direct_provider.hh"

namespace shiro::direct {

    class emulation : public direct_provider {
    public:
        std::tuple<bool, std::string> search(std::unordered_map<std::string, std::string> parameters) override;
        std::tuple<bool, std::string> download(int32_t beatmap_id, bool no_video) override;

        const std::string name() const override;

    };

}

#endif //SHIRO_EMULATION_HH
