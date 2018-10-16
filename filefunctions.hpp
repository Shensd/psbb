#ifndef FILE_FUNCTIONS_HPP
#define FILE_FUNCTIONS_HPP

#include <fstream>
#include <string>
#include <limits.h> // realpath()
#include <string>

#include <iostream>

bool get_file_exists(std::string path);
std::ifstream get_file(std::string path);
std::string get_file_content(std::string path);
std::string get_absolute_path(std::string file);
bool is_inside_path(std::string requested_file, std::string path);

#endif // !FILE_FUNCTIONS_HPP