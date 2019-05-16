#include "simpletcpsocket.hpp"

namespace sock {

    /**
     * Creates a TCP, IPV4 socket and returns it
     * 
     * @returns a new TCP, IPV4 socket
     */
    int SimpleTCPSocket::create_socket(void) {
        return socket(
            AF_INET,     // ipv4
            SOCK_STREAM, // tcp
            0            // protocol
        );
    }

    /**
     * Set the options for a given socket, allows port reuse and binding
     * 
     * @param sockfd socket to set options for
     * @return -1 if error
     */
    int SimpleTCPSocket::set_socket_options(int sockfd) {
        int options = 0;

        int setopt = setsockopt(
            sockfd,                      // socket
            SOL_SOCKET,                  // level (socket api level)
            SO_REUSEADDR | SO_REUSEPORT, // allow socket to bind, reuse of port permitted
            &options,                    // where to write return value
            sizeof(options)              // length of return value
        );
        
        if(setopt < 0) options = setopt;

        return options;
    }

    /**
     * Creates a socket address structure for the given port, uses ipv4 and 
     * allows for any address to connect
     * 
     * @param port port to use address structure on
     * @return new address structure
     */
    struct sockaddr_in SimpleTCPSocket::get_client_address(int port) {
        struct sockaddr_in addr;

        addr.sin_family = AF_INET;         // ipv4
        addr.sin_port = htons(port);       // userport
        addr.sin_addr.s_addr = INADDR_ANY; // allow any address to connect

        return addr;
    }

    /**
     * Bind a given socket to the given port with the given options via an 
     * address structure
     * 
     * @param sockfd socket to bind
     * @param addr address options to use
     * @param port port to bind to
     * @return -1 if error
     */
    int SimpleTCPSocket::set_binded_sock(int sockfd, struct sockaddr_in addr, int port) {
        int sock_bind = bind(
            sockfd,                    // socket
            (struct sockaddr*) &addr,  // address structure
            sizeof(addr)               // size of address structure
        );

        return sock_bind;
    }
    
    /**
     * set a given socket to listen at a given port, with a given max request
     * queue 
     * 
     * @param sockfd socket to listen with
     * @param max_queue max amount of waiting requests (default 50)
     * @return -1 if error 
     */
    int SimpleTCPSocket::set_socket_listen_port(int sockfd, int max_queue) {
        return listen(sockfd, max_queue);
    }

    SimpleTCPSocket::SimpleTCPSocket(int port) {
        SimpleTCPSocket::port = port;

        sockfd = create_socket();
        if(sockfd == -1) {error = true; return;}

        if(set_socket_options(sockfd) == -1) {
            error = true; 
            return;
        }

        addr = get_client_address(port);

        if(set_binded_sock(sockfd, addr, port) == -1) {
            error = true; 
            return;
        }

        if(set_socket_listen_port(sockfd, 50) == -1) {
            error = true;
            return;
        }
    }

    /**
     * Waits until a valid connection is available and accepts it, if the
     * connection it accepts is invalid it will discard it and wait for a
     * valid one
     */
    void SimpleTCPSocket::wait_for_connection(void) {
        socklen_t peer_addr_size = sizeof(struct sockaddr_in);
        connection = accept(
            sockfd,                        // socket
            (struct sockaddr*) &peer_addr, // connection addr
            &peer_addr_size                // size of connection addr
        );

        if(connection == -1) {
            discard_connection();
            wait_for_connection();
        }
    }

    /**
     * Close the current connection
     */
    void SimpleTCPSocket::discard_connection(void) {
        close(connection);
        connection = -1;
    }

    std::string SimpleTCPSocket::get_ip_human(void) {
        return inet_ntoa(addr.sin_addr);
    }

    /**
     * Read a given number of bytes into a buffer from the current connection,
     * if there is no connection then no bytes will be read
     * 
     */
    void SimpleTCPSocket::read_bytes(int amount, char* buffer) {
        if(connection == -1) return;

        int valread = recv(
            connection, // connection sock
            buffer,     // data buffer
            amount,     // data read length
            0           // flags (none)
        );
    }

    /**
     * Write a string to the current connection, if there is no connection then
     * no bytes will be written
     * 
     * @param content string to be wrote to connection
     */
    void SimpleTCPSocket::operator<<(std::string content) {
        if(connection == -1) return;

        send(
            connection,       // socket
            content.c_str(),  // response content
            content.length(), // response length
            0                 // flags (none)
        );
    }

    /**
     * Read one byte from the current connection into a stream, if there
     * is no connection then no bytes will be read
     * 
     * @param stream stream to write byte into
     */
    void SimpleTCPSocket::operator>>(std::ostream stream) {
        if(connection == -1) return;

        char* buffer = new char[1];
        recv(connection, buffer, 1, 0);
        stream.write(buffer, 1);
    }

}