#ifndef NET_HANDLER_HPP
#define NET_HANDLER_HPP

// network
#include <arpa/inet.h>  // inet_ntoa()
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // read(), close()

#include <thread>
#include <future>
#include <chrono>
#include <vector>

#include <errno.h>
#include <stdexcept>

#include <iostream>

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
    struct sockaddr_in addr, peer_addr;

    int sockfd;
    int opt = 1;

    int sock_bind;
    int listener;
    
    NetHandler(struct SERVER_PARAMS*);
    ~NetHandler();

    std::string (*request_callback)(int, sockaddr_in*, std::string) = nullptr;

    void set_request_callback(std::string (*f)(int, sockaddr_in*, std::string));
    int init_server(int* error);
    int start_server(int* error);
private:
    bool callback_set = false;
    bool server_init = false;
    bool listening = false;

    int init(int* error);
    int do_bind(int* error);
    int do_listen(int* error);

    int max_threads = 200;
    int current_threads = 0;

    std::vector<std::future<int>> states;

    std::string get_request_content(int sock);
    void do_outbound_socket_response(int sock, std::string content);

    static int do_test(int sock, sockaddr_in* peer_addr, std::string content, std::string (*request_callback)(int, sockaddr_in*, std::string));
};

#endif // !NET_HANDLER_HPP
