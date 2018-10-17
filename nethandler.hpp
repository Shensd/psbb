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

    bool uses_default_port;
    bool uses_default_home_dir;
    bool uses_default_index;
    bool uses_default_error;
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

    void (*request_callback)(int, sockaddr_in*) = nullptr;

    void set_request_callback(void (*f)(int, sockaddr_in*));
    int init_server(void);
    int start_server(void);
private:
    bool callback_set = false;
    bool server_init = false;

    void error(std::string);
    void init(void);
    int do_bind(void);
    void do_listen(void);
};

#endif // !NET_HANDLER_HPP