#ifndef SHIRO_LOCATION_INFO_HH
#define SHIRO_LOCATION_INFO_HH

#include <string>
#include <utility>

#include "country_ids.hh"

namespace shiro::geoloc {

    class location_info {
    public:
        uint8_t country;
        float latitude;
        float longitude;

        explicit location_info(uint8_t country = (uint8_t) country_id::JP, float latitude = 0.0f, float longitude = 0.0f)
                : country(country)
                , latitude(latitude)
                , longitude(longitude) {
            // Initializer in initializer list
        }

    };

    static location_info invalid_location;

}

#endif  // SHIRO_LOCATION_INFO_HH
