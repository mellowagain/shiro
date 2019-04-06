#ifndef SHIRO_CLIENT_SIDE_FLAGS_HH
#define SHIRO_CLIENT_SIDE_FLAGS_HH

#include <cstdint>

#include "../thirdparty/enum.hh"

namespace shiro::utils {

    // Up-to-date with osu!stable 20190401.2 (1st of April 2019)
    // Values provided by Cyuubi, thanks!

    BETTER_ENUM(
            client_side_flags, int32_t,
            cli_argument = 1 << 0,
            console_window = 1 << 1,
            interacting_process = 1 << 2,
            hq_osu_loaded = 1 << 3,
            existing_hq_osu_file = 1 << 4,
            debugger_override = 1 << 5,
            sdl2_loaded = 1 << 6,
            eay32_loaded = 1 << 7
    )

}

#endif  // SHIRO_CLIENT_SIDE_FLAGS_HH
