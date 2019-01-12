#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include <iostream>

#include "../net/nethandler.hpp"
#include "../func/stringfunctions.hpp"
#include "../func/filefunctions.hpp"

int process_args(int argc, char* argv[], struct SERVER_PARAMS* server);

#endif // !ARGUMENTS_HPP