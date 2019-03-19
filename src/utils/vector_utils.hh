#ifndef SHIRO_VECTOR_UTILS_HH
#define SHIRO_VECTOR_UTILS_HH

#include <cstdint>
#include <vector>

namespace shiro::utils {

    template <typename It>
    std::vector<std::pair<It, It>> chunk(It range_from, It range_to, const ptrdiff_t num) {
        const ptrdiff_t total { std::distance(range_from, range_to) };
        const ptrdiff_t portion { total / num };

        std::vector<std::pair<It,It>> chunks(num);

        It portion_end { range_from };

        std::generate(std::begin(chunks), std::end(chunks), [&portion_end, portion]() {
            It portion_start { portion_end };

            portion_end += portion;
            return std::make_pair(portion_start, portion_end);
        });

        chunks.back().second = range_to;

        return chunks;
    }

}

#endif  // SHIRO_VECTOR_UTILS_HH
