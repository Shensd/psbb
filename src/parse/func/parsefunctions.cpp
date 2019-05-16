#include "parsefunctions.hpp"

std::map<std::string, std::string> mime_type_dictionary = {
    {"css","text/css"},
    {"doc","application/msword"},
    {"gif","image/gif"},
    {"html","text/html"},
    {"ico","image/x-icon"},
    {"jar","application/java-archive"},
    {"jpeg","image/jpeg"},
    {"jpg","image/jpg"},
    {"js","application/javascript"},
    {"json","application/json"},
    {"mid","audio/midi"},
    {"midi","audio/midi"},
    {"oga","audio/ogg"},
    {"ogv","video/ogg"},
    {"ogx","application/ogg"},
    {"png","image/png"},
    {"rar","application/x-rar-compressed"},
    {"svg","image/svg+xml"},
    {"tif","image/tiff"},
    {"tiff","image/tiff"},
    {"wav","audio/x-wav"},
    {"weba","audio/webm"},
    {"webm","audio/webm"},
    {"xhtml","application/xhtml+xml"},
    {"xml","application/xml"},
    {"zip","application/zip"}
};

std::map<std::string, int> request_type_dictionary = {
    {"GET",     REQUEST_GET},
    {"HEAD",    REQUEST_HEAD},
    {"POST",    REQUEST_POST},
    {"PUT",     REQUEST_PUT},
    {"DELETE",  REQUEST_DELETE},
    {"TRACE",   REQUEST_TRACE},
    {"OPTIONS", REQUEST_OPTIONS},
    {"CONNECT", REQUEST_CONNECT},
    {"PATCH",   REQUEST_PATCH},
};

/**
 * get requested file path from http request, cleans input to
 * protect machine security, and modifies path to most correct
 * form
 * 
 * @param line first line of http request
 * @param server settings for parsing
 * @param raw return path with no modifications
 * @returns most correct path name without checking if file exists
 */
std::string parse::get_file_path(std::string line, struct SERVER_PARAMS* server, bool raw) {
    std::string path = stringutils::split(line, ' ')[1];

    // return path with no modifications
    if(raw) return path;

    // remove get parameters and create map for future use
    std::map<std::string, std::string> get_params;

    std::vector<std::string> request_split = stringutils::split(path, '?');

    // clean get parameters from path
    path = request_split[0];

    request_split = stringutils::subarr(request_split, 1, request_split.size());

    std::string param_str = stringutils::join(request_split, "?");

    for(std::string p : stringutils::split(param_str, '&')) {
        std::vector<std::string> param = stringutils::split(p, '=');

        std::string name = param[0];
        param = stringutils::subarr(param, param.size() - 1, param.size());
        std::string content = stringutils::join(param, "=");
        
        get_params.insert(
            std::pair<std::string, std::string>(
                name, 
                content
            )
        );
    }

    // if path is empty or a slash return index page
    if(path == "" || path == " " || path == "/") {
        path = server->index;
    }
    path = server->home_dir + path;

    return path;
}

/**
 * returns the request type of an http request
 * 
 * @param line first line of http request
 * @returns pair of request type string and definition number
 */
std::pair<std::string, int> parse::get_request_type(std::string line) {
    std::string type = stringutils::split(line, ' ').at(0);
    std::map<std::string, int>::iterator iterator = request_type_dictionary.find(type);

    int type_num;

    if(iterator != request_type_dictionary.end()) {
        type_num = iterator->second;
    } else {
        type_num = REQUEST_OTHER;
    }

    return std::pair<std::string, int>(type, type_num);
}

/**
 * get the mime type of a requested file
 * 
 * @param file file to get mime type of
 * @returns string of mime type for direct use in response
 */
std::string parse::get_mime_type(std::string file) {
    std::vector<std::string> broken = stringutils::split(file, '.');
    std::string extension = broken.at(broken.size() - 1);

    std::transform(
        extension.begin(), 
        extension.end(), 
        extension.begin(), 
        ::tolower
    );

    std::map<std::string, std::string>::iterator type = 
        mime_type_dictionary.find(extension);

    if(type != mime_type_dictionary.end()) {
        return type->second;
    } 
    return "text/html";
}

/**
 * checks if a request is good or not
 * 
 * checks if:
 *  - first line is valid (follows structure of type, path, http version)
 *  - request ends clean
 * 
 * @param request_lines request broken up into lines
 * @returns true if request is valid
 */
int parse::is_good_request(std::vector<std::string> request_lines, struct SERVER_PARAMS* server) {
    std::string path = get_file_path(request_lines[0], server);
    std::string home_absolute = fileutils::get_absolute_path(server->home_dir);

    if(fileutils::get_file_exists(path)) {
        if(!fileutils::is_inside_path(path, home_absolute)) {
            return 403;
        }
    }
    
    if(stringutils::split(request_lines[0], ' ').size() != 3) {
        return 400;
    }
    return 0;
}

/**
 * parse a given config file and put content into server struct
 * 
 * @param file_path path to config file
 * @param server server config to feed configuration into
 */
void parse::parse_config_file(std::string file_path, struct SERVER_PARAMS* server) {
    return;
}