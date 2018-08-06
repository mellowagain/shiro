#ifndef SHIRO_SPECTATOR_MANAGER_HH
#define SHIRO_SPECTATOR_MANAGER_HH

#include <memory>

#include "../users/user.hh"

namespace shiro::spectating::manager {

    extern std::vector<std::pair<std::shared_ptr<users::user>, std::shared_ptr<users::user>>> currently_spectating;

    void start_spectating(std::shared_ptr<users::user> user, std::shared_ptr<users::user> target);

    void stop_spectating(std::shared_ptr<users::user> user);

    bool is_spectating(std::shared_ptr<users::user> user);

    bool is_being_spectated(std::shared_ptr<users::user> user);

    std::vector<std::shared_ptr<users::user>> get_spectators(std::shared_ptr<users::user> user);

    std::shared_ptr<users::user> get_host(std::shared_ptr<users::user> user);

}

#endif //SHIRO_SPECTATOR_MANAGER_HH
