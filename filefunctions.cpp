#include "filefunctions.hpp"

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

