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

    simplesocket = new sock::SimpleTCPSocket(server->port);

    return simplesocket->error ? -1 : 0;
}

/**
 * listens for requests and sends them to callback function
 * 
 * @param error error code on fail
 * @returns -1 on error, 0 on success
 */
int NetHandler::listen(int* error) {

    listening = true;
    while(listening) {

        simplesocket->wait_for_connection();

        char buffer[4096];

        simplesocket->read_bytes(4096, buffer);

        std::string content(buffer);

        if(content.length() < 1) {
            simplesocket->discard_connection();
            continue;
        }

        *simplesocket << request_callback(simplesocket, content);

        simplesocket->discard_connection();
    }

    return 0;
}

/**
 * set the request callback
 * 
 * @param f function to set to request callback
 */
void NetHandler::set_request_callback(std::string(*f)(sock::SimpleTCPSocket*, std::string)) {
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

    listen(error);

    return 0;
}


NetHandler::~NetHandler() {
    listening = false;
    simplesocket->discard_connection();
}
