#include "exception.hh"

MPFD::Exception::Exception(std::string error) {
    Error = error;
}

MPFD::Exception::Exception(const MPFD::Exception& orig) {
    Error = orig.Error;
}

MPFD::Exception::~Exception() {

}

std::string MPFD::Exception::GetError() {
    return Error;
}
