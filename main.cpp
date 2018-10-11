// std
#include <string>
#include <stdlib.h>
#include <vector>
#include <sstream> // string stream
#include <iterator>
#include <map>

// network
#include <arpa/inet.h>  // inet_ntoa()
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>     // read(), close()

// debug
#include <iostream>
#include <errno.h>

// local
#include "definitions.h"

#define VERSION "0.0.1"

struct SERVER_PARAMS {
    int port;
    std::string home_dir;
    std::string index;
} server;

int error(std::string msg) {
    std::cout << msg << std::endl;
        std::cout << errno << std::endl;
    return -1;
}

void process_args(int argc, char* argv[]) {
    server.port = 8080;
    server.home_dir = "./www";
    server.index = "index.html";
}

std::vector<std::string> split(std::string str, char delim) {
    std::vector<std::string> lines;
    std::istringstream iss(str);
    std::string line;
    while(std::getline(iss, line, delim)) lines.push_back(line);
    return lines;
}

std::map<std::string, int> request_dictionary;

void create_dictionary(void) {
    request_dictionary.insert(std::pair<std::string, int>("GET",     REQUEST_GET));
    request_dictionary.insert(std::pair<std::string, int>("HEAD",    REQUEST_HEAD));
    request_dictionary.insert(std::pair<std::string, int>("POST",    REQUEST_POST));
    request_dictionary.insert(std::pair<std::string, int>("PUT",     REQUEST_PUT));
    request_dictionary.insert(std::pair<std::string, int>("DELETE",  REQUEST_DELETE));
    request_dictionary.insert(std::pair<std::string, int>("TRACE",   REQUEST_TRACE));
    request_dictionary.insert(std::pair<std::string, int>("OPTIONS", REQUEST_OPTIONS));
    request_dictionary.insert(std::pair<std::string, int>("CONNECT", REQUEST_CONNECT));
    request_dictionary.insert(std::pair<std::string, int>("PATCH",   REQUEST_PATCH));
}


std::pair<std::string, int> get_request_type(std::string line) {
    std::string type = split(line, ' ').at(0);
    return std::pair<std::string, int>(type, request_dictionary.find(type)->second);
}

void process_request(int sock, struct sockaddr_in* addr) {
    char buffer[1024] = {0};
    int valread = read(
        sock,   // connection sock
        buffer, // data buffer
        1024    // data read length
    );

    std::vector<std::string> lines = split((std::string)buffer, '\n');

    std::string ip = inet_ntoa(addr->sin_addr);
    std::pair<std::string, int> type = get_request_type(lines.at(0));

    std::cout << "REQUEST FROM " << ip << std::endl;
    std::cout << "TYPE         " << type.first << "(" << type.second << ")" << std::endl;
    // dont display last empty line
    for(unsigned int i = 0; i < lines.size() - 1; i++) {
        std::cout << " |  " << lines.at(i) << std::endl;
    }
    std::cout << std::endl;

    int status = 200;
    std::string response;
    response += "HTTP/1.1 200 OK\r\n";
    response += "Content-Length: 11\r\n";
    response += "Connection: close\r\n";
    response += "Content-type: text/html\r\n";
    response += "\r\n";
    response += "i love you\n\r\n\r\n";

    send(
        sock,              // socket
        response.c_str(),  // response content
        response.length(), // response length
        0                  // flags (none)
    );
    close(sock); // close connection whend done
}

void banner(std::string version) {
    std::cout << "_______  _______  _______  _______ "  << std::endl;
    std::cout << "|       ||       ||  _    ||  _    |" << std::endl;
    std::cout << "|    _  ||  _____|| |_|   || |_|   |" << std::endl;
    std::cout << "|   |_| || |_____ |       ||       |" << std::endl;
    std::cout << "|    ___||_____  ||  _   | |  _   | " << std::endl;
    std::cout << "|   |     _____| || |_|   || |_|   |" << std::endl;
    std::cout << "|___|    |_______||_______||_______|" << std::endl;
    std::cout << std::endl;
    std::cout << "VERSION " << version << " IS NOW RUNNING." << std::endl;
    std::cout << std::endl;
    std::cout << "waiting for connections..." << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    process_args(argc, argv);
    create_dictionary();

    int sockfd = socket(
        AF_INET,     // ipv4
        SOCK_STREAM, // tcp
        0            // protocol
    );

    if(sockfd == -1) return error("Unable to create socket");

    struct sockaddr_in addr, peer_addr;

    int opt = 1;

    int setopt = setsockopt(
        sockfd,                      // socket
        SOL_SOCKET,                  // level (socket api level)
        SO_REUSEADDR | SO_REUSEPORT, // allow socket to bind, reuse of port permitted
        &opt,                        // where to write return value
        sizeof(opt)                  // length of return value
    );

    if(setopt == -1) return error("Unable to set opt");

    addr.sin_family = AF_INET;          // ipv4
    addr.sin_port = htons(server.port); // userport
    addr.sin_addr.s_addr = INADDR_ANY;  // allow any address to connect

    int sock_bind = bind(
        sockfd,                    // socket
        (struct sockaddr*) &addr,  // address structure
        sizeof(addr)               // size of address structure
    );

    if(sock_bind == -1) return error("Unable to bind");

    int listener = listen(
        sockfd, // socket
        50      // max length of queue
    );

    if(listener == -1) return error("Unable to listen");

    banner(VERSION);

    while(true) {
        socklen_t peer_addr_size = sizeof(struct sockaddr_in);
        int connection = accept(
            sockfd,                        // socket
            (struct sockaddr*) &peer_addr, // connection addr
            &peer_addr_size                // size of connection addr
        );
        if(connection == -1) return error("Bad connection");
        
        process_request(connection, &peer_addr);
    }

    return 0;
}