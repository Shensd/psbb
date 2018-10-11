#ifndef SERVER_PARAMS_HPP
#define SERVER_PARAMS_HPP

#include <string>

struct SERVER_PARAMS {
    int port;
    
    std::string home_dir;
    std::string index;
    std::string error;
};

#endif // !SERVER_PARAMS_HPP