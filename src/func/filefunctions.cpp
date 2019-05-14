#include "filefunctions.hpp"

namespace fileutils {
    /**
     * return if a file exists or not
     * 
     * @param path path of file
     * @returns boolean of whether file exists or not
     */
    bool get_file_exists(std::string path) {
        std::ifstream test(path.c_str());
        bool exists = test.is_open();
        test.close();
        return exists;
    }

    /**
     * get file at specified path
     * note: this function does not check if a file exists before
     * returning
     * 
     * @param path path of file
     * @returns specified file
     */
    std::ifstream get_file(std::string path) {
        std::ifstream file(path.c_str());
        return file;
    }

    /**
     * get file content from a file at a given path
     * 
     * @param path path of file
     * @returns file content
     * if file does not exist then an empty string is returned
     */
    std::string get_file_content(std::string path) {
        std::ifstream file = get_file(path);

        std::string file_contents = "";

        if(!get_file_exists(path)) return file_contents;

        while(!file.eof()) {
            std::string temp;
            std::getline(file, temp);
            file_contents = file_contents + temp + "\n";
        }

        file.close();

        return file_contents;
    }

    /**
     * returns the absolute path of a file, used for sandboxing
     * 
     * @param path relative path of file
     * @returns string of absolute path
     */
    std::string get_absolute_path(std::string file) {
        char* absolute_path = realpath(file.c_str(), NULL);
        std::string absolute = absolute_path;
        delete absolute_path;
        return absolute;
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
    bool is_inside_path(std::string requested_file, std::string path) {
        std::string requested_absolute_path = get_absolute_path(requested_file);

        if(requested_absolute_path.length() < path.length()) return false;

        std::string cut = requested_absolute_path.substr(0, path.length());
        
        return cut == path;
    }
}