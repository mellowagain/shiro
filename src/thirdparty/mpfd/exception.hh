#ifndef SHIRO_EXCEPTION_HH
#define SHIRO_EXCEPTION_HH

#include <string>
#include <iostream>
#include <errno.h>

namespace MPFD {

    class Exception {
    public:
        Exception(std::string error);
        Exception(const Exception& orig);
        virtual ~Exception();

        std::string GetError();

    private:
        std::string Error;

    };
}

#endif //SHIRO_EXCEPTION_HH
