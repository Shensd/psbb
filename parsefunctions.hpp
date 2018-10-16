#ifndef PARSE_FUNCTIONS_HPP
#define PARSE_FUNCTIONS_HPP

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <errno.h>

#include "nethandler.hpp"
#include "definitions.hpp"
#include "stringfunctions.hpp"
#include "filefunctions.hpp"

std::string get_file_path(std::string line, struct SERVER_PARAMS* server, bool raw=false);
std::pair<std::string, int> get_request_type(std::string line);
std::string get_mime_type(std::string file);
int is_good_request(std::vector<std::string> request_lines, struct SERVER_PARAMS* server);


#endif // !PARSE_FUNCTIONS_HPP