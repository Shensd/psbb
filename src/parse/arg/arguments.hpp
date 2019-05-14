#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include <map>

#include "../../net/handler/nethandler.hpp"
#include "../../func/stringfunctions.hpp"
#include "../../func/filefunctions.hpp"

#include "flags/flag.hpp"

namespace arg {

    namespace {

        std::vector<std::string> get_arg_vector(int argc, char* argv[]);
        bool is_tack_flag(std::string flag);
        std::string get_tack_flag_content(std::string flag);
        std::map<std::string, Flag*> get_flag_map(std::vector<Flag*> flags);

    }

    std::string process_args(int argc, char* argv[], struct SERVER_PARAMS* server, std::vector<Flag*> flags);

    int process_args(int argc, char* argv[], struct SERVER_PARAMS* server);

}

#endif // !ARGUMENTS_HPP