#ifndef PARSE_FUNCTIONS_HPP
#define PARSE_FUNCTIONS_HPP

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <errno.h>

#include "../net/nethandler.hpp"
#include "../definitions.hpp"
#include "../func/stringfunctions.hpp"
#include "../func/filefunctions.hpp"

namespace parse {
    std::string get_file_path(std::string line, struct SERVER_PARAMS* server, bool raw=false);
    std::pair<std::string, int> get_request_type(std::string line);
    std::string get_mime_type(std::string file);
    int is_good_request(std::vector<std::string> request_lines, struct SERVER_PARAMS* server);

    void parse_config_file(std::string file_path, struct SERVER_PARAMS* server);
}


#endif // !PARSE_FUNCTIONS_HPP