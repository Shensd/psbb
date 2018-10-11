#ifndef NET_HANDLER_HPP
#define NET_HANDLER_HPP

#include <arpa/inet.h>  // inet_ntoa()
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // read(), close()

#include <errno.h>

#include "serverparams.hpp"

class NetHandler {
public:
    struct SERVER_PARAMS* server;
    struct sockaddr_in addr, peer_addr;

    int sockfd;
    int opt = 1;

    int sock_bind;
    int listener;
    
    NetHandler(struct SERVER_PARAMS*, void (*f)(int, socklen_t));

    void (*request_callback)(int, socklen_t);
private:
    void error(std::string, int);
    void init();
    void bind();
    void listen();
};

#endif // !NET_HANDLER_HPP