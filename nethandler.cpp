#include "nethandler.hpp"

NetHandler::NetHandler(struct SERVER_PARAMS* server) {
    NetHandler::server = server;
}

/**
 * initializes socket and fills address structure
 * 
 * @param error error code on fail
 * @returns -1 on error, 0 on success
 */
int NetHandler::init(int* error) {
    sockfd = socket(
        AF_INET,     // ipv4
        SOCK_STREAM, // tcp
        0            // protocol
    );
    if(sockfd == -1) {
        *error = ERROR_SOCKET_CREATION;
        return -1;
    }

    int opt = 1;

    int setopt = setsockopt(
        sockfd,                      // socket
        SOL_SOCKET,                  // level (socket api level)
        SO_REUSEADDR | SO_REUSEPORT, // allow socket to bind, reuse of port permitted
        &opt,                        // where to write return value
        sizeof(opt)                  // length of return value
    );

    if(setopt == -1) {
        *error = ERROR_SET_OPTIONS;
        return -1;
    }

    addr.sin_family = AF_INET;          // ipv4
    addr.sin_port = htons(server->port); // userport
    addr.sin_addr.s_addr = INADDR_ANY;  // allow any address to connect

    return 0;
}

/**
 * bind socket to port specified in address structure
 * 
 * @param error error code on fail
 * @returns -1 on error, 0 on success
 */
int NetHandler::do_bind(int* error) {
    sock_bind = bind(
        sockfd,                    // socket
        (struct sockaddr*) &addr,  // address structure
        sizeof(addr)               // size of address structure
    );

    if(sock_bind == -1) {
        *error = ERROR_UNABLE_TO_BIND;
        return -1;
    }

    return sock_bind;
}

/**
 * listens for requests and sends them to callback function
 * 
 * @param error error code on fail
 * @returns -1 on error, 0 on success
 */
int NetHandler::do_listen(int* error) {
    listener = listen(
        sockfd, // socket
        50      // max length of queue
    );

    if(listener == -1) {
        *error = ERROR_UNABLE_TO_LISTEN;
        return -1;
    }

    listening = true;
    while(listening) {
        socklen_t peer_addr_size = sizeof(struct sockaddr_in);
        int connection = accept(
            sockfd,                        // socket
            (struct sockaddr*) &peer_addr, // connection addr
            &peer_addr_size                // size of connection addr
        );
        if(connection == -1) {
            *error = ERROR_BAD_REQUEST;
            return -1;
        }

        process_overflow_requests();

        if(current_threads < max_threads) {
            states.push_back(std::thread(do_test, connection, &peer_addr, request_callback));
            current_threads++;
        } else {
            struct REQUEST_HOLD rh = { connection, &peer_addr };
            request_queue.push_back(&rh);
        }

        /**
         * IDEA
         * 
         * Keep track of sockets, not the threads, and just check if the passed
         * connection has been closed by the socket, if it has then join/kill
         * the corresponding thread and clear from vector
         */

        //iterate through currently running threads to check for any that are finished
        for(int i = 0; i < states.size(); i++) {
            if(!states.at(i).joinable()) {
                std::cout << "THREAD DONE" << std::endl;
                current_threads--;
                states.erase(states.begin() + i);
                i--;
            }
        }

        std::cout << "CURRENT THREADS : " << current_threads << std::endl;
        std::cout << "THREAD VEC SIZE : " << states.size() << std::endl;
    }

    return 0;
}

/**
 * process request queue when there are thread spots available
 */
void NetHandler::process_overflow_requests(void) {
    for(int i = current_threads; i < max_threads; i++) {
        if(request_queue.size() == 0) {
            return;
        }
        struct REQUEST_HOLD* rh = request_queue.at(0);
        states.push_back(std::thread(do_test, rh->connection, rh->peer_addr, request_callback));
        current_threads++;

        request_queue.erase(request_queue.begin() + i);
    }
} 

/**
 * a very poorly named function for the time being
 * 
 * actually -> worker function for requests threads
 * 
 * @param connection socket connection
 * @param peer_addr address structure for connection
 * @param content content of request
 * @param request_callback function for processing_requsts
 * @returns 8 (thread convention)
 */
int NetHandler::do_test(int connection, sockaddr_in* peer_addr, std::string (*request_callback)(int, sockaddr_in*, std::string)) {
    char buffer[4096] = {0};
    int valread = recv(
        connection, // connection sock
        buffer, // data buffer
        4096,   // data read length
        0
    );
    
    std::string content = buffer;

    std::string response = request_callback(connection, peer_addr, content);
    
    if(content.length() < 1) {
        close(connection);
        return 8;
    }

    send(
        connection,        // socket
        response.c_str(),  // response content
        response.length(), // response length
        0                  // flags (none)
    );

    close(connection); // close connection when done

    return 8;
}

/**
 * set the request callback
 * 
 * @param f function to set to request callback
 */
void NetHandler::set_request_callback(std::string(*f)(int, sockaddr_in*, std::string)) {
    request_callback = f;
    callback_set = true;
}

/**
 * creates socket and binds
 * 
 * @param error error code on fail
 * @returns -1 on error, 0 on success
 */
int NetHandler::init_server(int* error) {

    if(NetHandler::init(error) == -1) {
        return -1;
    }
    if(NetHandler::do_bind(error) == -1) {
        return -1;
    }

    return 0;
}

/**
 * creates socket, binds, and starts listeneing
 * 
 * @param error error code on fail
 * @returns -1 on error, 0 on success
 */
int NetHandler::start_server(int* error) {
    if(!callback_set) {
        *error = ERROR_CALLBACK_NOT_SET;
        return -1;
    }

    do_listen(error);

    return 0;
}

/**
 * read request from socket and convert std::string
 * 
 * @param sock socket to grab content from
 * @returns string of content
 */
std::string NetHandler::get_request_content(int sockfd) {
    char buffer[4096] = {0};
    int valread = recv(
        sockfd, // connection sock
        buffer, // data buffer
        4096,   // data read length
        0
    );

    return (std::string)buffer;
}

/**
 * send content to outbound socket
 * 
 * will close socket when done with operation
 * 
 * @param sock socket to send content to
 * @param content content to send
 */
void NetHandler::do_outbound_socket_response(int sockfd, std::string content) {
    if(content.length() < 1) {
        close(sockfd);
        return;
    }
    send(
        sockfd,           // socket
        content.c_str(),  // response content
        content.length(), // response length
        0                 // flags (none)
    );
    close(sockfd); // close connection whend done
}

NetHandler::~NetHandler() {
    listening = false;
    close(sockfd);
}
