#include "response.hpp"

/**
 * takes a header struct and converts it to string response
 * 
 * @param headers header structure with response content
 * @returns response string 
 */
std::string build_response(struct HEADERS* headers) {
    std::string response = "";

    response += headers->response_code + "\r\n";
    if(headers->content_length > 0) {
        response += "Content-Length: " + std::to_string(headers->content_length) + "\r\n";
    }
    response += "Connection: " + headers->connection + "\r\n";
    response += "Content-type: " + headers->content_type + "\r\n";
    response += "Server: " + headers->server + "\r\n";
    response += "\r\n";
    if(headers->body.length() > 0) {
        response += headers->body;
        response += "\r\n\r\n";
    } else {
        response += "\r\n";
    }

    return response;
}

/**
 * returns the reponse for a standard file serve
 * 
 * @param path path to file to be served
 * @returns string of response
 */
std::string get_file_serve_response(std::string path) {
    std::string file_content = get_file_content(path);

    struct HEADERS headers;

    headers.content_length = file_content.length();
    headers.connection     = "close";
    headers.content_type   = get_mime_type(path) + "; charset=UTF-8";
    headers.server         = "PSBB/" + (std::string)VERSION + " (xinU)";
    headers.body           = file_content;

    std::string response = build_response(&headers);

    return response;
}