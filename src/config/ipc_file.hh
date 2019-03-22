#ifndef SHIRO_IPC_FILE_HH
#define SHIRO_IPC_FILE_HH

#include <string>

namespace shiro::config::ipc {

    extern std::string backend_url;
    extern std::string frontend_url;
    extern std::string beatmap_url;

    void parse();

}

#endif //SHIRO_IPC_FILE_HH
