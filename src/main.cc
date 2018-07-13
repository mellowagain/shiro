#include <cstdlib>

#include "shiro.hh"

int main(int argc, char **argv) {
    std::atexit(shiro::destroy);
    return shiro::init(argc, argv);
}
