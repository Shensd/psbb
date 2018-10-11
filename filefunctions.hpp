#ifndef FILE_FUNCTIONS_HPP
#define FILE_FUNCTIONS_HPP

#include <fstream>
#include <string>

bool get_file_exists(std::string path) {
    std::ifstream test(path.c_str());
    bool exists = test.is_open();
    test.close();
    return exists;
}

std::ifstream get_file(std::string path) {
    std::ifstream file(path.c_str());
    return file;
}

std::string get_file_content(std::string path) {
    std::ifstream file = get_file(path);

    std::string file_contents = "";
    while(!file.eof()) {
        std::string temp;
        std::getline(file, temp);
        file_contents = file_contents + temp + "\n";
    }

    file.close();

    return file_contents;
}

#endif // !FILE_FUNCTIONS_HPP