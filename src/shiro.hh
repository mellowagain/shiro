#ifndef SHIRO_SHIRO_HH
#define SHIRO_SHIRO_HH

#include <memory>

#include "database/database.hh"

namespace shiro {

    static inline std::shared_ptr<database> db_connection = nullptr;

    int init(int argc, char **argv);

    void destroy();

}

#endif //SHIRO_SHIRO_HH
