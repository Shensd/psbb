#ifndef NET_HANDLER_HPP
#define NET_HANDLER_HPP

#include <arpa/inet.h>  // inet_ntoa()
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // read(), close()

#include <errno.h>
#include <stdexcept>

struct SERVER_PARAMS {
    int port;
    
    std::string home_dir;
    std::string index;
    std::string error;
};

class NetHandler {
public:
    struct SERVER_PARAMS* server;
    struct sockaddr_in addr, peer_addr;

    int sockfd;
    int opt = 1;

    int sock_bind;
    int listener;
    
    NetHandler(struct SERVER_PARAMS*);

    void do_listen(void (*f)(int, sockaddr_in*));
private:
    void error(std::string);
    void init(void);
    void do_bind(void);
};

#endif // !NET_HANDLER_HPP