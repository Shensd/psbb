#include "requesthandler.hpp"

RequestHandler::RequestHandler() {}

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
std::pair<std::string, int> RequestHandler::get_head_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    std::string path_raw = get_file_path(lines.at(0), server, true);
    std::string path = get_file_path(lines.at(0), server);
    std::string ip = inet_ntoa(addr->sin_addr);

    bool exists = get_file_exists(path);

    if(!exists) {
        path = (std::string)DEFAULT_PAGE_DIR + "/404.html";
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
std::pair<std::string, int> RequestHandler::get_get_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    std::string path_raw = get_file_path(lines.at(0), server, true);
    std::string path = get_file_path(lines.at(0), server);
    std::string ip = inet_ntoa(addr->sin_addr);

    struct HEADERS headers;

    std::string file_contents;
    int status;

    bool exists = get_file_exists(path);

    if(exists) {
        file_contents = get_file_content(path);
        headers.response_code = RESPONSE_200;
        status = 200;
    } else {
        return get_404_not_found();
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
std::pair<std::string, int> RequestHandler::get_post_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
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
std::pair<std::string, int> RequestHandler::get_put_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
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
std::pair<std::string, int> RequestHandler::get_delete_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
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
std::pair<std::string, int> RequestHandler::get_trace_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
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
std::pair<std::string, int> RequestHandler::get_options_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
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
std::pair<std::string, int> RequestHandler::get_connect_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
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
std::pair<std::string, int> RequestHandler::get_patch_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
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
std::pair<std::string, int> RequestHandler::get_other_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    std::string response = "wha, what? why are you even here?\n";
    return std::pair<std::string, int>(response, 418);
}

/**
 * reponse for a bad request
 * 
 * @returns pair of reponse content to be sent and status code 
 */
std::pair<std::string, int> RequestHandler::get_400_bad_request(void) {
    std::string response = get_file_serve_response((std::string)DEFAULT_PAGE_DIR + "/400.html");
    int status = 400;
    return std::pair<std::string, int>(response, status);
}

/**
 * reponse for a forbidden file
 * 
 * @returns pair of reponse content to be sent and status code 
 */
std::pair<std::string, int> RequestHandler::get_403_forbidden(void) {
    std::string response = get_file_serve_response((std::string)DEFAULT_PAGE_DIR + "/403.html");
    int status = 403;
    return std::pair<std::string, int>(response, status);
}

/**
 * reponse for a file not found
 * 
 * @returns pair of reponse content to be sent and status code 
 */
std::pair<std::string, int> RequestHandler::get_404_not_found(void) {
    std::string response = get_file_serve_response((std::string)DEFAULT_PAGE_DIR + "/404.html");
    int status = 404;
    return std::pair<std::string, int>(response, status);
}

/**
 * handles a given content request, takes full request lines and returns
 * the response content
 * 
 * @param lines request lines
 * @returns pair of respond string and status code
 */
std::pair<std::string, int> RequestHandler::handle_request(std::vector<std::string> lines, struct sockaddr_in* addr, struct SERVER_PARAMS* server) {
    std::pair<std::string, int> type = get_request_type(lines.at(0));
    std::pair<std::string, int> result;

    int error = is_good_request(lines, server);
    if(error > 0) {
        if (error == 400) result = get_400_bad_request();
        if (error == 403) result = get_403_forbidden();
    } else {
        switch(type.second) {
            case REQUEST_GET:
                result = get_get_request(lines, addr, server);
                break;
            case REQUEST_POST:
                result = get_post_request(lines, addr, server);
                break;
            case REQUEST_HEAD:
                result = get_head_request(lines, addr, server);
                break;
            case REQUEST_PUT:
                result = get_put_request(lines, addr, server);
                break;
            case REQUEST_DELETE:
                result = get_delete_request(lines, addr, server);
                break;
            case REQUEST_TRACE:
                result = get_trace_request(lines, addr, server);
                break;
            case REQUEST_OPTIONS:
                result = get_options_request(lines, addr, server);
                break;
            case REQUEST_CONNECT:
                result = get_connect_request(lines, addr, server);
                break;
            case REQUEST_PATCH:
                result = get_patch_request(lines, addr, server);
                break;
            default:
                result = get_other_request(lines, addr, server);
                break;
        }
    }

    return result;
}