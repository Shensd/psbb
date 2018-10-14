#include "requesthandlers.hpp"

/**
 * response for HEAD http request
 * 
 * get all information in the style of a GET request
 * without any body content
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @returns pair of reponse content to be sent and status code
 */
std::pair<std::string, int> get_head_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    std::string path_raw = get_file_path(lines.at(0), server, true);
    std::string path = get_file_path(lines.at(0), server);
    std::string ip = inet_ntoa(addr->sin_addr);

    bool exists = get_file_exists(path);

    if(!exists) {
        path = server->home_dir + server->error;
    }

    struct HEADERS headers;

    int status;

    if(exists) {
        headers.response_code = RESPONSE_200;
        status = 200;
    } else {
        headers.response_code = RESPONSE_404;
        status = 404;
    }

    headers.content_length = 0;
    headers.connection     = "close";
    headers.content_type   = get_mime_type(path) + "; charset=UTF-8";
    headers.server         = "PSBB/" + (std::string)VERSION + " (xinU)";
    headers.body           = "";

    std::string response = build_response(&headers);

    return std::pair<std::string, int>(response, status);
}

/**
 * response for GET http request
 * 
 * standard request, headers and body of file content,
 * if a file is not found it will return with user specified
 * error page
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @returns pair of reponse content to be sent and status code
 */
std::pair<std::string, int> get_get_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    std::string path_raw = get_file_path(lines.at(0), server, true);
    std::string path = get_file_path(lines.at(0), server);
    std::string ip = inet_ntoa(addr->sin_addr);

    bool exists = get_file_exists(path);

    if(!exists) {
        path = server->home_dir + server->error;
    }

    struct HEADERS headers;

    std::string file_contents;
    int status;

    if(exists) {
        file_contents = get_file_content(path);
        headers.response_code = RESPONSE_200;
        status = 200;
    } else {
        file_contents = get_file_content(path);
        headers.response_code = RESPONSE_404;
        status = 404;
    }

    headers.content_length = file_contents.length();
    headers.connection     = "close";
    headers.content_type   = get_mime_type(path) + "; charset=UTF-8";
    headers.server         = "PSBB/" + (std::string)VERSION + " (xinU)";
    headers.body           = file_contents;

    std::string response = build_response(&headers);

    return std::pair<std::string, int>(response, status);
}

/**
 * response for POST http request
 * 
 * currently the same response as get_get_request,
 * possibly changed in future if post parameters need
 * to be used
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @returns pair of reponse content to be sent and status code
 */
std::pair<std::string, int> get_post_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    return get_get_request(lines, addr, server);
}

/**
 * response for PUT http request
 * 
 * currently the same response as get_get_request,
 * possibly changed in future if need for request type
 * is found
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @returns pair of reponse content to be sent and status code
 */
std::pair<std::string, int> get_put_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    return get_get_request(lines, addr, server);
}

/**
 * response for DELETE http request
 * 
 * currently the same response as get_get_request,
 * possibly changed in future if need for request type
 * is found
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @returns pair of reponse content to be sent and status code
 */
std::pair<std::string, int> get_delete_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    return get_get_request(lines, addr, server);
}

/**
 * response for TRACE http request
 * 
 * currently the same response as get_get_request,
 * possibly changed in future if need for request type
 * is found
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @returns pair of reponse content to be sent and status code
 */
std::pair<std::string, int> get_trace_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    return get_get_request(lines, addr, server);
}

/**
 * response for OPTIONS http request
 * 
 * currently the same response as get_get_request,
 * possibly changed in future if need for request type
 * is found
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @returns pair of reponse content to be sent and status code
 */
std::pair<std::string, int> get_options_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    return get_get_request(lines, addr, server);
}

/**
 * response for CONNECT http request
 * 
 * currently the same response as get_get_request,
 * possibly changed in future if need for request type
 * is found
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @returns pair of reponse content to be sent and status code
 */
std::pair<std::string, int> get_connect_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    return get_get_request(lines, addr, server);
}

/**
 * response for PATCH http request
 * 
 * currently the same response as get_get_request,
 * possibly changed in future if need for request type
 * is found
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @returns pair of reponse content to be sent and status code
 */
std::pair<std::string, int> get_patch_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    return get_get_request(lines, addr, server);
}

/**
 * response for any non standard http request
 * 
 * if someone gets this then they are looking for trouble
 * 
 * @param lines request lines from get_request_content_lines()
 * @param addr socket address structure
 * @returns pair of reponse content to be sent and status code
 */
std::pair<std::string, int> get_other_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    std::string response = "wha, what? why are you even here?\n";
    return std::pair<std::string, int>(response, 418);
}

/**
 * reponse for a bad request
 * 
 * @returns pair of reponse content to be sent and status code 
 */
std::pair<std::string, int> get_400_bad_request(void) {
    std::string response = get_file_serve_response((std::string)DEFAULT_PAGE_DIR + "/400.html");
    int status = 400;
    return std::pair<std::string, int>(response, status);
}