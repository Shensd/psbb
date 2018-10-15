#ifndef REQUEST_HANDLERS_HPP
#define REQUEST_HANDLERS_HPP

#include <map>
#include <vector>
#include <string>

#include <netinet/in.h> // sockaddr_in

#include "response.hpp"
#include "definitions.hpp"
#include "stringfunctions.hpp"
#include "filefunctions.hpp"
#include "parsefunctions.hpp"
#include "nethandler.hpp"

std::pair<std::string, int> get_head_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server);
std::pair<std::string, int> get_get_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server);
std::pair<std::string, int> get_post_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server);
std::pair<std::string, int> get_put_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server);
std::pair<std::string, int> get_delete_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server);
std::pair<std::string, int> get_trace_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server);
std::pair<std::string, int> get_options_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server);
std::pair<std::string, int> get_connect_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server);
std::pair<std::string, int> get_patch_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server);
std::pair<std::string, int> get_other_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server);
std::pair<std::string, int> get_400_bad_request(void);

#endif // !REQUEST_HANDLERS_HPP