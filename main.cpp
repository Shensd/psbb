// std
#include <string>
#include <stdlib.h>
#include <vector>
#include <sstream> // string stream
#include <iterator>
#include <fstream>
#include <algorithm>

// network
#include <arpa/inet.h>  // inet_ntoa()
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // read(), close()

// debug
#include <iostream>
#include <errno.h>

// local
#include "response.hpp"
#include "definitions.hpp"
#include "stringfunctions.hpp"
#include "filefunctions.hpp"
#include "arguments.hpp"
#include "nethandler.hpp"
#include "requesthandler.hpp"

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
 * read request from socket and convert to vector of strings
 * 
 * @param sock socket to grab content from
 * @returns vector of strings, content has been broken up
 * for easier processing
 */
std::vector<std::string> get_request_content_lines(int sock) {
    char buffer[1024] = {0};
    int valread = read(
        sock,   // connection sock
        buffer, // data buffer
        1024    // data read length
    );

    std::vector<std::string> lines = split((std::string)buffer, '\n');

    return lines;
}

/**
 * prints debug information about request to stdout
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @param status status of response
 */
void do_debug_log(std::vector<std::string> lines, struct sockaddr_in* addr, int status) {
    std::string path_raw = get_file_path(lines.at(0), &server, true);
    std::string path = get_file_path(lines.at(0), &server);
    std::string ip = inet_ntoa(addr->sin_addr);

    std::string one_line_request;

    one_line_request = join(lines, " ");
    one_line_request = replace(one_line_request, "\r", "");

    std::cout << ip 
              << " :: " 
              << one_line_request 
              << " :: " 
              << std::to_string(status) 
              << std::endl;
}

/**
 * send content to outbound socket
 * 
 * will close socket when done with operation
 * 
 * @param sock socket to send content to
 * @param content content to send
 */
void do_outbound_socket_response(int sock, std::string content) {
    send(
        sock,              // socket
        content.c_str(),  // response content
        content.length(), // response length
        0                  // flags (none)
    );
    close(sock); // close connection whend done
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
void do_request(int sock, struct sockaddr_in* addr) {

    std::vector<std::string> lines = get_request_content_lines(sock);

    std::pair<std::string, int> result = request_handler.handle_request(
        lines, 
        addr,
        &server
    );

    do_debug_log(lines, addr, result.second);

    do_outbound_socket_response(sock, result.first);
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
    std::cout << "webdir absolute path : " << get_absolute_path(server->home_dir) << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if(process_args(argc, argv, &server) < 0) {
        return -1;
    }

    NetHandler nethandler = NetHandler(&server);

    nethandler.set_request_callback(&do_request);

    do_banner(VERSION, &server);

    if(nethandler.init_server() < 0) {
        std::cout << "Unable to init server" << std::endl;
        return -1;
    } 

    std::cout << "waiting for connections..." << std::endl;
    std::cout << std::endl;

    if(nethandler.start_server() < 0) {
        std::cout << "Issue in running server" << std::endl;
        return -1;
    }

    return 0;
}