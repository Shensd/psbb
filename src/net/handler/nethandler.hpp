#ifndef NET_HANDLER_HPP
#define NET_HANDLER_HPP

#include <thread>
#include <future>
#include <chrono>
#include <vector>

#include <errno.h>
#include <stdexcept>

#include <iostream>

#include "../sock/simpletcpsocket.hpp"

struct SERVER_PARAMS {
    int port;
    
    std::string home_dir;
    std::string index;
    std::string error;

    bool uses_default_port;
    bool uses_default_home_dir;
    bool uses_default_index;
    bool uses_default_error;
};

const int ERROR_SOCKET_CREATION  = 235;
const int ERROR_SET_OPTIONS      = 236;
const int ERROR_UNABLE_TO_LISTEN = 237;
const int ERROR_BAD_REQUEST      = 238;
const int ERROR_UNABLE_TO_BIND   = 239;
const int ERROR_CALLBACK_NOT_SET = 240;

class NetHandler {
public:
    struct SERVER_PARAMS* server;
    
    NetHandler(struct SERVER_PARAMS*);
    ~NetHandler();

    std::string (*request_callback)(sock::SimpleTCPSocket*, std::string) = nullptr;

    void set_request_callback(std::string (*f)(sock::SimpleTCPSocket*, std::string));
    int init_server(int* error);
    int start_server(int* error);

    sock::SimpleTCPSocket* simplesocket;
private:
    bool callback_set = false;
    bool server_init = false;
    bool listening = false;

    int init(int* error);
    int listen(int* error);

    std::string get_request_content(int sock);
    void do_outbound_socket_response(int sock, std::string content);
};

#endif // !NET_HANDLER_HPP
