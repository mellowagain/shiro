#ifndef SHIRO_OPPAI_WRAPPER_HH
#define SHIRO_OPPAI_WRAPPER_HH

#include "../../thirdparty/oppai.hh"
#include "../../beatmaps/beatmap.hh"
#include "../../scores/score.hh"

namespace shiro::pp {

    class oppai_wrapper {
    private:
        ezpp_t ez = nullptr;

    public:
        oppai_wrapper(beatmaps::beatmap beatmap, scores::score score);
        ~oppai_wrapper();

        float calculate();

    };

}

#endif //SHIRO_OPPAI_WRAPPER_HH
