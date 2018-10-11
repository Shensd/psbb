#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include "serverparams.hpp"

void process_args(int argc, char* argv[], struct SERVER_PARAMS* server) {
    server->port = 8080;
    server->home_dir = "./www";
    server->index = "/index.html";
    server->error = "/error.html";
}

#endif // !ARGUMENTS_HPP