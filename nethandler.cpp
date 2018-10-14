#include "nethandler.hpp"

NetHandler::NetHandler(struct SERVER_PARAMS* server) {
    NetHandler::server = server;

    NetHandler::init();
    NetHandler::do_bind();
}

/**
 * error handler, throws message along with errno
 * 
 * @param errormsg error message to be thrown 
 */
void NetHandler::error(std::string errormsg) {
    throw std::runtime_error(
        errormsg + " (" + std::to_string(errno) + ")"
    );
}

/**
 * initializes socket and fills address structure
 */
void NetHandler::init(void) {
    sockfd = socket(
        AF_INET,     // ipv4
        SOCK_STREAM, // tcp
        0            // protocol
    );
    if(sockfd == -1) error("Unable to create socket");

    int opt = 1;

    int setopt = setsockopt(
        sockfd,                      // socket
        SOL_SOCKET,                  // level (socket api level)
        SO_REUSEADDR | SO_REUSEPORT, // allow socket to bind, reuse of port permitted
        &opt,                        // where to write return value
        sizeof(opt)                  // length of return value
    );

    if(setopt == -1) error("Unable to set options");

    addr.sin_family = AF_INET;          // ipv4
    addr.sin_port = htons(server->port); // userport
    addr.sin_addr.s_addr = INADDR_ANY;  // allow any address to connect
}

/**
 * bind socket to port specified in address structure
 */
void NetHandler::do_bind(void) {
    sock_bind = bind(
        sockfd,                    // socket
        (struct sockaddr*) &addr,  // address structure
        sizeof(addr)               // size of address structure
    );

    if(sock_bind == -1) error("Unable to bind");
}

/**
 * listens for requests and sends them to callback function
 * 
 * @param request_callback callback function for requests
 */
void NetHandler::do_listen(void (*request_callback)(int, sockaddr_in*)) {
    listener = listen(
        sockfd, // socket
        50      // max length of queue
    );

    if(listener == -1) error("Unable to listen");

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