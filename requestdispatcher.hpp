#ifndef REQUEST_DISPATCHER_HPP
#define REQUEST_DISPATCHER_HPP

#include <thread>
#include <vector>
#include <string>

// network
#include <arpa/inet.h>  // inet_ntoa()
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // read(), close()

#include "nethandler.hpp"

class RequestDispatcher {
public:
    RequestDispatcher(int max_threads);
    void process_request(int sock);


    void set_max_threads(int threads);
    int get_max_threads(void);
    int get_current_threads(void);
private:
    std::vector<std::thread*> threads;

    int max_threads = 50;
    int current_threads = 0;
};

#endif // !REQUEST_DISPATCHER_HPP