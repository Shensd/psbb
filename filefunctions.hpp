#ifndef FILE_FUNCTIONS_HPP
#define FILE_FUNCTIONS_HPP

#include <fstream>
#include <string>

bool get_file_exists(std::string path);
std::ifstream get_file(std::string path);
std::string get_file_content(std::string path);

#endif // !FILE_FUNCTIONS_HPP