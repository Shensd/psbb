#include "arguments.hpp"

/**
 * takes args for given program and converts data from them
 * into a server struct 
 * 
 * COMMAND LINE ARGUMENTS WILL EVENTUALLY BE PHASED OUT
 * THIS IS ONLY LEFT HERE CURRENTLY FOR DEBUGGING
 * 
 * @param argc args length
 * @param argv array of args
 * @param server structure for information to be copyied to
 */
int process_args(int argc, char* argv[], struct SERVER_PARAMS* server) {

    // -h, --help
    //    display list of command line commands
    // -p, --port
    //    set the port that server runs on
    // -d, --home-dir 
    //    set the home directory
    // -i, --index
    //    set the index page

    bool port_set     = false;
    bool home_dir_set = false;
    bool index_set    = false;


    for(int i = 1; i < argc; i++) {
        std::string current_arg = argv[i];
        std::string arg_lower = stringutils::to_lower(current_arg);

        if(arg_lower == "-p" || arg_lower == "--port") {
            port_set = true;
            if(argc > i + 1) {
                try {
                    server->port = std::stoi(argv[i+1], nullptr); 
                } catch(const std::invalid_argument &ia) {
                    std::cout << "No valid port given, defaulting to port 80" << std::endl;
                    port_set = false;
                } catch(const std::out_of_range &oor) {
                    std::cout << "Given port too large, defaulting to port 80" << std::endl;
                    port_set = false;
                }
            } else {
                std::cout << "Port flag set but no argument given, exiting..." << std::endl;
                return -1;
            }
            i++;
            continue;
        }

        if(arg_lower == "-d" || arg_lower == "--home-dir") {
            home_dir_set = true;
            if(argc > i + 1) {
                if(fileutils::get_file_exists(argv[i+1])) {
                    if(server->home_dir.at(server->home_dir.length()-1) != '/') {
                        server->home_dir = (std::string)argv[i+1] + "/";
                    } else {
                        server->home_dir = argv[i+1];
                    }
                } else {
                    std::cout << "Directory " 
                          << argv[i+1] 
                          << " does not exist, defaulting to ./www/" 
                          << std::endl;
                    home_dir_set = false;
                }
                
            } else {
                std::cout << "Home directory flag set but no argument given, exiting..." << std::endl;
                return -1;
            }
            i++;
            continue;
        }

        if(arg_lower == "-i" || arg_lower == "--index") {
            index_set = true;
            if(argc > i + 1) {
                server->index = argv[i+1];
            } else {
                std::cout << "Index flag set but no argument given, exiting..." << std::endl;
                return -1;
            }
            i++;
            continue;
        }
    }

    if(!port_set) {
        server->port = 80;
        server->uses_default_port = true;
    }
    if(!home_dir_set) {
        server->home_dir = "./www/";
        server->uses_default_home_dir = true;
    }
    if(!index_set) {
        server->index = "index.html";
        server->uses_default_index = true;
    }

    std::cout << "Using port " << server->port << std::endl;
    std::cout << "Using home directory " << server->home_dir << std::endl;
    std::cout << "Using index file " << server->index << std::endl;

    return 0;
}