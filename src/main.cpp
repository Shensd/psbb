// std
#include <string>
#include <stdlib.h>
#include <vector>
#include <thread>

// network
#include <arpa/inet.h>  // inet_ntoa()
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in

// debug
#include <iostream>
#include <errno.h>

// local
#include "net/response/response.hpp"
#include "definitions.hpp"
#include "func/stringfunctions.hpp"
#include "func/filefunctions.hpp"
#include "parse/arg/arguments.hpp"
#include "net/handler/nethandler.hpp"
#include "net/handler/requesthandler.hpp"
#include "net/dispatch/requestdispatcher.hpp"
#include "net/sock/simpletcpsocket.hpp"

#include "parse/arg/flags/flag.hpp"
#include "parse/arg/flags/flaghome.hpp"
#include "parse/arg/flags/flagindex.hpp"
#include "parse/arg/flags/flagport.hpp"

struct SERVER_PARAMS server; 

/**
 * prints error message to stdout with errno
 * 
 * @param msg error message to print
 * @returns used for main method compatability
 */
int error(std::string msg) {
    std::cout << msg << std::endl;
    std::cout << errno << std::endl;
    return -1;
}

/**
 * prints debug information about request to stdout
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @param status status of response
 */
void do_debug_log(std::vector<std::string> lines, sock::SimpleTCPSocket* simplesocket, int status) {
    std::string path_raw = parse::get_file_path(lines.at(0), &server, true);
    std::string path = parse::get_file_path(lines.at(0), &server);
    std::string ip = simplesocket->get_ip_human();

    std::string one_line_request;

    one_line_request = stringutils::join(lines, " ");
    one_line_request = stringutils::replace(one_line_request, "\r", "");

    std::cout << ip 
              << " :: " 
              << one_line_request 
              << " :: " 
              << std::to_string(status) 
              << std::endl;
}

/**
 * listener callback for server requests, takes request
 * and forwards it to specific type function and then
 * sends back response to outbound socket
 * 
 * @param sock socket for request
 * @param addr socket request structure
 */
RequestHandler request_handler = RequestHandler();
static std::string do_request(sock::SimpleTCPSocket* simplesocket, std::string content) {

    if(content.length() < 1) {
        return "";
    }

    std::vector<std::string> lines = stringutils::split((std::string)content, '\n');

    std::pair<std::string, int> result = request_handler.handle_request(
        lines, 
        simplesocket,
        &server
    );

    do_debug_log(lines, simplesocket, result.second);

    return result.first;
}

/**
 * prints program banner in stdout
 * 
 * @param version used for version number
 */
void do_banner(std::string version, struct SERVER_PARAMS* server) {
    std::cout << " _______  _______  _______  _______ " << std::endl;
    std::cout << "|       ||       ||  _    ||  _    |" << std::endl;
    std::cout << "|    _  ||  _____|| |_|   || |_|   |" << std::endl;
    std::cout << "|   |_| || |_____ |       ||       |" << std::endl;
    std::cout << "|    ___||_____  ||  _   | |  _   | " << std::endl;
    std::cout << "|   |     _____| || |_|   || |_|   |" << std::endl;
    std::cout << "|___|    |_______||_______||_______|" << std::endl;
    std::cout << std::endl;
    std::cout << "VERSION " << version << " IS NOW RUNNING." << std::endl;
    std::cout << std::endl;
    std::cout << "running on port      : " << server->port << std::endl;
    std::cout << "web directory is     : " << server->home_dir << std::endl;
    std::cout << "webdir absolute path : " << fileutils::get_absolute_path(server->home_dir) << std::endl;
    std::cout << std::endl;
}


int main(int argc, char* argv[]) {
    
    std::vector<Flag*> cli_flags;
    cli_flags.push_back(new FlagHome());
    cli_flags.push_back(new FlagIndex());
    cli_flags.push_back(new FlagPort());

    std::string arg_error = arg::process_args(argc, argv, &server, cli_flags);

    if(arg_error.compare("") != 0) {
        std::cout << "ERROR " << arg_error << std::endl;
        return -1;
    }

    do_banner(VERSION, &server);

    NetHandler* nethandler = new NetHandler(&server);
    nethandler->set_request_callback(&do_request);

    int error = 0;
    if(nethandler->init_server(&error) < 0) {
        std::cout << "Unable to init server " << error << std::endl;
        return -1;
    } 

    std::cout << "waiting for connections..." << std::endl;
    std::cout << std::endl;

    if(nethandler->start_server(&error) < 0) {
        std::cout << "Issue in running server " << error << std::endl;
        return -1;
    }

    return 0;
}