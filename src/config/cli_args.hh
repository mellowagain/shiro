#ifndef SHIRO_CLI_ARGS_HH
#define SHIRO_CLI_ARGS_HH

#include <tuple>

#include "../thirdparty/cli11.hh"

namespace shiro::config::cli {

    extern CLI::App cli_app;

    void parse(int argc, char **argv);

    std::tuple<int, char**> get_args();

}

#endif //SHIRO_CLI_ARGS_HH
