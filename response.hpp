#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <string>

#include "filefunctions.hpp"
#include "parsefunctions.hpp"

struct HEADERS {
    int content_length;
    
    std::string response_code;
    std::string connection;
    std::string content_type;
    std::string server;

    std::string body;
};

std::string build_response(struct HEADERS* headers);
std::string get_file_serve_response(std::string path);

#endif // !HEADERS_HPP