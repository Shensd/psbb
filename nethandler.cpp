#include "nethandler.hpp"

NetHandler::NetHandler(
        struct SERVER_PARAMS* server,
        void (*request_callback)(int, socklen_t)
    ) {
    NetHandler::server = server;
    NetHandler::request_callback = request_callback;

    init();
    bind();
    listen();
}

void NetHandler::init(void) {
    int sockfd = socket(
        AF_INET,     // ipv4
        SOCK_STREAM, // tcp
        0            // protocol
    );

    if(sockfd == -1) return error("Unable to create socket");

    struct sockaddr_in addr, peer_addr;

    int opt = 1;

    int setopt = setsockopt(
        sockfd,                      // socket
        SOL_SOCKET,                  // level (socket api level)
        SO_REUSEADDR | SO_REUSEPORT, // allow socket to bind, reuse of port permitted
        &opt,                        // where to write return value
        sizeof(opt)                  // length of return value
    );

    if(setopt == -1) return error("Unable to set opt");

    addr.sin_family = AF_INET;          // ipv4
    addr.sin_port = htons(server.port); // userport
    addr.sin_addr.s_addr = INADDR_ANY;  // allow any address to connect
}

void NetHandler::bind(void) {
    sock_bind = bind(
        sockfd,                    // socket
        (struct sockaddr*) &addr,  // address structure
        sizeof(addr)               // size of address structure
    );

    if(sock_bind == -1) return error("Unable to bind");
}

void NetHandler::listen(void) {
    listener = listen(
        sockfd, // socket
        50      // max length of queue
    );

    if(listener == -1) return error("Unable to listen");

    while(true) {
        socklen_t peer_addr_size = sizeof(struct sockaddr_in);
        int connection = accept(
            sockfd,                        // socket
            (struct sockaddr*) &peer_addr, // connection addr
            &peer_addr_size                // size of connection addr
        );
        if(connection == -1) return error("Bad connection");
        
        request_callback(connection, &peer_addr);
    }
}