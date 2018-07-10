#ifndef SHIRO_CLI_ARGS_HH
#define SHIRO_CLI_ARGS_HH

#include <tuple>

#include "../thirdparty/cli11.hh"

namespace shiro::config::cli {

    static ::CLI::App cli_app = CLI::App("C++ osu!Bancho re-implementation", "shiro");

    void parse(int argc, char **argv);

    std::tuple<int, char**> get_args();

}

#endif //SHIRO_CLI_ARGS_HH
