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

    /**
     * IDEA
     * 
     * wowie zowie i think the answer was under our nose the whole time
     * 
     * we looked at promises for a second so why dont we just use those?
     * we are waiting for a value, so why not just wait for the promise to be 
     * filled then kill the thread?
     * 
     * this slightly worries me because i feel i may be missing a big piece that
     * i saw in the past but eh lets give it a try
     */

    listening = true;
    while(listening) {
        //iterate through currently running threads to check for any that are finished
        for(int i = 0; i < states.size(); i++) {
            std::cout << "CHECKED " << states.size() << std::endl;
            std::future_status fs = states.at(i).first.wait_for(std::chrono::seconds(0));
            if(fs == std::future_status::ready) {
                
                struct REQUEST_FINISHED* rf = states.at(i).first.get();
                send_finished_request(rf);
                states.at(i).second.join();

                std::cout << "before " << states.size() << std::endl;
                if(states.size() == 1) {
                    states.clear();
                } else {
                    states.erase(states.begin() + i);
                }
                std::cout << "after " << states.size() << std::endl;

                i--;
                current_threads--;
            }
        }

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

        struct REQUEST_HOLD rh = { connection, &peer_addr, request_callback };

        if(current_threads < max_threads) {
            process_overflow_requests();
            
            std::promise<struct REQUEST_FINISHED*> out;
            std::shared_future<struct REQUEST_FINISHED*> worker_future = out.get_future();

            current_threads++;

            states.push_back(std::pair<std::shared_future<struct REQUEST_FINISHED*>, std::thread>(worker_future, std::thread(double_test, &rh, std::move(out))));
        } else {
            request_queue.push_back(&rh);
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

        std::promise<struct REQUEST_FINISHED*> out;
        std::shared_future<struct REQUEST_FINISHED*> worker_future = out.get_future();

        current_threads++;

        states.push_back(std::pair<std::shared_future<struct REQUEST_FINISHED*>, std::thread>(worker_future, std::thread(double_test, rh, std::move(out))));

        request_queue.erase(request_queue.begin() + i);
    }
} 

/**
 * wow i just love poorly written functions
 * 
 * another test function for threading
 * 
 * @param request info for request
 * @param out promise for output value
 */
void NetHandler::double_test(struct REQUEST_HOLD* request, std::promise<struct REQUEST_FINISHED*> out) {
    int connection = request->connection;
    struct sockaddr_in* peer_addr = request->peer_addr;

    char buffer[4096] = {0};
    int valread = recv(
        connection, // connection sock
        buffer, // data buffer
        4096,   // data read length
        0
    );
    
    std::string content = buffer;

    if(content.length() < 1) {
        close(connection);
        struct REQUEST_FINISHED rf= {request, (std::string)""};
        out.set_value(&rf);
    }

    std::string response = request->request_callback(connection, peer_addr, content);

    struct REQUEST_FINISHED rf= {request, response};
    out.set_value(&rf);
}

/**
 * sends a finished request struct to the contained sock
 * 
 * @param request structure of request data
 * @returns 8
 */
int NetHandler::send_finished_request(struct REQUEST_FINISHED* request) {
    
    int connection = request->connection_info->connection;
    std::string response = request->content;

    if(response.length() < 1) return 8;

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
