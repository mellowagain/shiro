#ifndef SHIRO_GEOLOC_HH
#define SHIRO_GEOLOC_HH

#include <string>

#include "../thirdparty/cache/cache.hh"
#include "location_info.hh"

namespace shiro::geoloc {

    // Limit cache to 256 locations, oldest ones will be overriden once we go over this
    static caches::fixed_sized_cache<std::string, location_info> location_cache(256);

    location_info get_location(const std::string &ip_address);

    size_t callback(void *raw_data, size_t size, size_t memory, std::string *ptr);

}

#endif //SHIRO_GEOLOC_HH
