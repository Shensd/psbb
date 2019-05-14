#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <string>

#include "../../func/filefunctions.hpp"
#include "../../parse/func/parsefunctions.hpp"

namespace response {
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
    void write_file_serve_headers(std::string path, struct HEADERS* headers);
}

#endif // !HEADERS_HPP