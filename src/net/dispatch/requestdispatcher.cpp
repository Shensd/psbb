#include "requestdispatcher.hpp"


RequestDispatcher::RequestDispatcher(int max_threads) {
    RequestDispatcher::max_threads = max_threads;
}

/**
 * creates worker threads for server
 * 
 * @param server server parameters
 * @param f callback function for requests
 * 
 */
void RequestDispatcher::create_threads(struct SERVER_PARAMS* server, std::string (*f)(int, sockaddr_in*, std::string)) {
    for(int i = 0; i < max_threads; i++) {

        NetHandler nethandler(server);
        nethandler.set_request_callback(f);

        std::thread handler(worker, &nethandler);
        handler.detach();
        threads.push_back(std::pair<NetHandler*, std::thread*>(&nethandler, &handler));
    }
}

/**
 * destroys all threads 
 * 
 * may god rest their souls
 */
void RequestDispatcher::destroy_threads() {
    for(std::pair<NetHandler*, std::thread*> th : threads) {
        delete th.first;
        th.second->join();
    }
    threads.clear();
}   

/**
 * method used for server threads
 * 
 * @param server server parameters
 * @param f callback function
 */
void RequestDispatcher::worker(NetHandler* nethandler) {

    int error = 0;
    if(nethandler->init_server(&error) < 0) {
        std::cout << "Unable to init server " << error << std::endl;
        return;
    } 

    std::cout << "waiting for connections..." << std::endl;
    std::cout << std::endl;

    if(nethandler->start_server(&error) < 0) {
        std::cout << "Issue in running server " << error << std::endl;
        return;
    }
}

/**
 * set the max number of threads allowed
 * 
 * (this function should eventually)
 * 
 * @param threads max number of threads
 */
void RequestDispatcher::set_max_threads(int threads) {
    RequestDispatcher::max_threads = threads;
}

/**
 * returns the currently allowed max number of threads
 * 
 * @returns max number of threads
 */
int RequestDispatcher::get_max_threads(void) {
    return max_threads;
}

/**
 * get the number of currently running threads
 * 
 * @returns number of currently running threads
 */
int RequestDispatcher::get_current_threads(void) {
    return current_threads;
}