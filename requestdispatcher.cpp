#include "requestdispatcher.hpp"


RequestDispatcher::RequestDispatcher(int max_threads) {

}

/**
 * takes a socket and designates it a response thread
 * 
 * @param sock socket of which to process
 * @param addr address structure
 * @param content request content
 * @param process function of which to process request
 */
void RequestDispatcher::process_request(int sock, sockaddr_in* addr, std::string content, std::string(*process)(int, sockaddr_in*, std::string)) {
    
    if(current_threads == max_threads) {

    }

    current_threads++;
    std::thread worker = new std::thread(process(sock, addr, content));
    threads.push_back(worker);
}

/**
 * set the max number of threads allowed
 * 
 * (this function should eventually)
 * 
 * @param threads max number of threads
 */
void RequestDispatcher::set_max_threads(int threads) {
    RequestDispatcher::threads = threads;
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