#include "arguments.hpp"

/**
 * takes args for given program and converts data from them
 * into a server struct 
 * 
 * @param argc args length
 * @param argv array of args
 * @param server structure for information to be copyied to
 */
void process_args(int argc, char* argv[], struct SERVER_PARAMS* server) {

    bool port_set = false;

    for(int i = 1; i < argc; i++) {
        std::string current_arg = argv[i];
        if(to_lower(current_arg) == "-p" || to_lower(current_arg) == "--port") {
            port_set = true;
            if(argc > i + 1) {
                try {
                    server->port = std::stoi(argv[i+1], nullptr); 
                } catch(const std::invalid_argument &ia) {
                    server->port = 8080;
                } catch(const std::out_of_range &oor) {
                    server->port = 8080;
                }
            }
        }
    }

    if(!port_set) {
        server->port = 80;
    }

    server->home_dir = "./www";
    server->index = "/index.html";
    server->error = "/error.html";
}