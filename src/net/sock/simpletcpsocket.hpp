#ifndef SIMPLE_SOCKET_HPP
#define SIMPLE_SOCKET_HPP

// network
#include <arpa/inet.h>  // inet_ntoa()
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // read(), close()

#include <string>
#include <iostream>

namespace sock {

    class SimpleTCPSocket {
    private:
        int port;
        int sockfd = -1, connection = -1;

        struct sockaddr_in addr, peer_addr;
        int create_socket(void);
        int set_socket_options(int sockfd);
        struct sockaddr_in get_client_address(int port);
        int set_binded_sock(int sockfd, struct sockaddr_in addr, int port);
        int set_socket_listen_port(int sockfd, int max_queue=50);

    public:
        bool error = false;

        SimpleTCPSocket(int port);

        void wait_for_connection(void);
        void discard_connection(void);

        void read_bytes(int, char*);
        void operator<<(std::string);
        void operator>>(std::ostream);
    };
}

#endif // !SIMPLE_SOCKET_HPP