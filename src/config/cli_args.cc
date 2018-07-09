#include "../thirdparty/loguru.hh"
#include "cli_args.hh"

static int args_count = 0;
static char **args_vector {};

void shiro::config::cli::parse(int argc, char **argv) {
    args_count = argc;
    args_vector = argv;

    try {
        cli_app.parse(argc, argv);
    } catch (const CLI::ParseError &ex) {
        LOG_S(FATAL) << "Unable to parse command line arguments: " << ex.what() << ".";
    }
}

std::tuple<int, char**> shiro::config::cli::get_args() {
    return std::make_tuple(args_count, args_vector);
}
