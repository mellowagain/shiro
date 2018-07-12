#ifndef SHIRO_GEOLOC_HH
#define SHIRO_GEOLOC_HH

#include <string>

namespace shiro::geoloc {

    uint8_t get_country(float longitude, float latitude);

    size_t callback(void *raw_data, size_t size, size_t memory, std::string *ptr);

}

#endif //SHIRO_GEOLOC_HH
