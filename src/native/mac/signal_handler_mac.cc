#include "../signal_handler.hh"

#if defined(__APPLE__)
    #include "../linux/signal_handler_linux.cc"
#endif
