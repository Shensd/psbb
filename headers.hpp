#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <string>

struct HEADERS {
    int content_length;
    
    std::string response_code;
    std::string connection;
    std::string content_type;
    std::string server;

    std::string body;
};

#endif // !HEADERS_HPP