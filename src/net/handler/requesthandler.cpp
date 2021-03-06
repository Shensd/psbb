#include "requesthandler.hpp"

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
std::pair<std::string, int> get_head_request(
    std::vector<std::string> lines, 
    sock::SimpleTCPSocket* simplesocket,
    struct SERVER_PARAMS* server, 
    RequestHandler* handler) {

    std::string path_raw = parse::get_file_path(lines.at(0), server, true);
    std::string path = parse::get_file_path(lines.at(0), server);
    std::string ip = simplesocket->get_ip_human();

    bool exists = fileutils::get_file_exists(path);

    if(!exists) {
        path = (std::string)DEFAULT_PAGE_DIR + "/404.html";
    }

    struct response::HEADERS headers;

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
    headers.content_type   = parse::get_mime_type(path) + "; charset=UTF-8";
    headers.server         = "PSBB/" + (std::string)VERSION + " (xinU)";
    headers.body           = "";

    std::string response = response::build_response(&headers);

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
std::pair<std::string, int> get_get_request(
    std::vector<std::string> lines, 
    sock::SimpleTCPSocket* simplesocket,
    struct SERVER_PARAMS* server, 
    RequestHandler* handler) {

    std::string path_raw = parse::get_file_path(lines.at(0), server, true);
    std::string path = parse::get_file_path(lines.at(0), server);
    std::string ip = simplesocket->get_ip_human();

    struct response::HEADERS headers;

    std::string file_contents;
    int status;

    bool exists = fileutils::get_file_exists(path);

    if(exists) {
        file_contents = fileutils::get_file_content(path);
        headers.response_code = RESPONSE_200;
        status = 200;
    } else {
        return handler->get_404_not_found();
    }

    headers.content_length = file_contents.length();
    headers.connection     = "close";
    headers.content_type   = parse::get_mime_type(path) + "; charset=UTF-8";
    headers.server         = "PSBB/" + (std::string)VERSION + " (xinU)";
    headers.body           = file_contents;

    std::string response = response::build_response(&headers);

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
std::pair<std::string, int> get_post_request(
    std::vector<std::string> lines, 
    sock::SimpleTCPSocket* simplesocket,
    struct SERVER_PARAMS* server, 
    RequestHandler* handler) {
        
    return get_get_request(lines, simplesocket, server, handler);
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
std::pair<std::string, int> get_put_request(
    std::vector<std::string> lines, 
    sock::SimpleTCPSocket* simplesocket,
    struct SERVER_PARAMS* server, 
    RequestHandler* handler) {
        
    return get_get_request(lines, simplesocket, server, handler);
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
std::pair<std::string, int> get_delete_request(
    std::vector<std::string> lines, 
    sock::SimpleTCPSocket* simplesocket,
    struct SERVER_PARAMS* server, 
    RequestHandler* handler) {
        
    return get_get_request(lines, simplesocket, server, handler);
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
std::pair<std::string, int> get_trace_request(
    std::vector<std::string> lines, 
    sock::SimpleTCPSocket* simplesocket,
    struct SERVER_PARAMS* server, 
    RequestHandler* handler) {
        
    return get_get_request(lines, simplesocket, server, handler);
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
std::pair<std::string, int> get_options_request(
    std::vector<std::string> lines, 
    sock::SimpleTCPSocket* simplesocket,
    struct SERVER_PARAMS* server, 
    RequestHandler* handler) {
        
    return get_get_request(lines, simplesocket, server, handler);
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
std::pair<std::string, int> get_connect_request(
    std::vector<std::string> lines, 
    sock::SimpleTCPSocket* simplesocket,
    struct SERVER_PARAMS* server, 
    RequestHandler* handler) {
        
    return get_get_request(lines, simplesocket, server, handler);
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
std::pair<std::string, int> get_patch_request(
    std::vector<std::string> lines, 
    sock::SimpleTCPSocket* simplesocket, 
    struct SERVER_PARAMS* server, 
    RequestHandler* handler) {
        
    return get_get_request(lines, simplesocket, server, handler);
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
std::pair<std::string, int> get_other_request(
    std::vector<std::string> lines, 
    sock::SimpleTCPSocket* simplesocket,
    struct SERVER_PARAMS* server, 
    RequestHandler* handler) {
        
    return get_get_request(lines, simplesocket, server, handler);
}

/**
 * reponse for a bad request
 * 
 * @returns pair of reponse content to be sent and status code 
 */
std::pair<std::string, int> RequestHandler::get_400_bad_request(void) {
    struct response::HEADERS headers;
    
    headers.response_code = RESPONSE_400;
    response::write_file_serve_headers((std::string)DEFAULT_PAGE_DIR + "400.html", &headers);
    
    std::string response = response::build_response(&headers);
    int status = 400;

    return std::pair<std::string, int>(response, status);
}

/**
 * reponse for a forbidden file
 * 
 * @returns pair of reponse content to be sent and status code 
 */
std::pair<std::string, int> RequestHandler::get_403_forbidden(void) {
    struct response::HEADERS headers;
    
    headers.response_code = RESPONSE_403;
    response::write_file_serve_headers((std::string)DEFAULT_PAGE_DIR + "403.html", &headers);
    
    std::string response = response::build_response(&headers);
    int status = 403;

    return std::pair<std::string, int>(response, status);
}

/**
 * reponse for a file not found
 * 
 * @returns pair of reponse content to be sent and status code 
 */
std::pair<std::string, int> RequestHandler::get_404_not_found(void) {
    struct response::HEADERS headers;
    
    headers.response_code = RESPONSE_404;
    response::write_file_serve_headers((std::string)DEFAULT_PAGE_DIR + "404.html", &headers);
    
    std::string response = response::build_response(&headers);
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
std::pair<std::string, int> RequestHandler::handle_request(
    std::vector<std::string> lines, 
    sock::SimpleTCPSocket* simplesocket,
    struct SERVER_PARAMS* server) {

    std::pair<std::string, int> type = parse::get_request_type(lines.at(0));
    std::pair<std::string, int> result;

    int error = parse::is_good_request(lines, server);
    if(error > 0) {
        if (error == 400) result = get_400_bad_request();
        if (error == 403) result = get_403_forbidden();
    } else {

        if(type.second > 0 && type.second < request_parser_list.size()) {
            result = request_parser_list.at(type.second)(lines, simplesocket, server, this);
        } else {
            result = get_other_request(lines, simplesocket, server, this);
        }

    }

    return result;
}

RequestHandler::RequestHandler() {
    request_parser_list = {
        &get_get_request,
        &get_head_request,
        &get_post_request,
        &get_put_request,
        &get_delete_request,
        &get_trace_request,
        &get_options_request,
        &get_connect_request,
        &get_patch_request,
        &get_other_request
    };
}