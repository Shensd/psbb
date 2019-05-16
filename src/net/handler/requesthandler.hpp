#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <map>
#include <vector>
#include <string>

#include <netinet/in.h> // sockaddr_in

#include "../response/response.hpp"
#include "../../definitions.hpp"
#include "../../func/stringfunctions.hpp"
#include "../../func/filefunctions.hpp"
#include "../../parse/func/parsefunctions.hpp"
#include "nethandler.hpp"
#include "../sock/simpletcpsocket.hpp"

class RequestHandler {
private: 
 
    using ptr_request_parser = std::pair<std::string, int>(*)(std::vector<std::string>, sock::SimpleTCPSocket*, struct SERVER_PARAMS*, RequestHandler*);

    std::vector<ptr_request_parser> request_parser_list;

    friend std::pair<std::string, int> get_head_request(std::vector<std::string> lines, 
                                                        sock::SimpleTCPSocket* simplesocket, 
                                                        struct SERVER_PARAMS*,
                                                        RequestHandler* handler);
    friend std::pair<std::string, int> get_get_request(std::vector<std::string> lines, 
                                                        sock::SimpleTCPSocket* simplesocket, 
                                                        struct SERVER_PARAMS*,
                                                        RequestHandler* handler);
    friend std::pair<std::string, int> get_post_request(std::vector<std::string> lines, 
                                                        sock::SimpleTCPSocket* simplesocket, 
                                                        struct SERVER_PARAMS*,
                                                        RequestHandler* handler);
    friend std::pair<std::string, int> get_put_request(std::vector<std::string> lines, 
                                                        sock::SimpleTCPSocket* simplesocket, 
                                                        struct SERVER_PARAMS*,
                                                        RequestHandler* handler);
    friend std::pair<std::string, int> get_delete_request(std::vector<std::string> lines, 
                                                        sock::SimpleTCPSocket* simplesocket, 
                                                        struct SERVER_PARAMS*,
                                                        RequestHandler* handler);
    friend std::pair<std::string, int> get_trace_request(std::vector<std::string> lines, 
                                                        sock::SimpleTCPSocket* simplesocket, 
                                                        struct SERVER_PARAMS*,
                                                        RequestHandler* handler);
    friend std::pair<std::string, int> get_options_request(std::vector<std::string> lines, 
                                                        sock::SimpleTCPSocket* simplesocket, 
                                                        struct SERVER_PARAMS*,
                                                        RequestHandler* handler);
    friend std::pair<std::string, int> get_connect_request(std::vector<std::string> lines, 
                                                        sock::SimpleTCPSocket* simplesocket, 
                                                        struct SERVER_PARAMS*,
                                                        RequestHandler* handler);
    friend std::pair<std::string, int> get_patch_request(std::vector<std::string> lines, 
                                                        sock::SimpleTCPSocket* simplesocket, 
                                                        struct SERVER_PARAMS*,
                                                        RequestHandler* handler);
    friend std::pair<std::string, int> get_other_request(std::vector<std::string> lines, 
                                                        sock::SimpleTCPSocket* simplesocket, 
                                                        struct SERVER_PARAMS*,
                                                        RequestHandler* handler);
    
    std::pair<std::string, int> get_400_bad_request(void);
    std::pair<std::string, int> get_403_forbidden(void);
    std::pair<std::string, int> get_404_not_found(void);
public:
    RequestHandler();

    std::pair<std::string, int> handle_request(std::vector<std::string>, sock::SimpleTCPSocket*, struct SERVER_PARAMS*);
};


#endif // !REQUEST_HANDLER_HPP