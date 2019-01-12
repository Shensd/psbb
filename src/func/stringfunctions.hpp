#ifndef STRING_FUNCTIONS_HPP
#define STRING_FUNCTIONS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

namespace stringutils {
    std::vector<std::string> split(std::string str, char delim);
    std::string replace(std::string str, std::string from, std::string to);
    std::string join(std::vector<std::string> strings, std::string bridge);
    std::string to_lower(std::string);

    template<class T>
    std::vector<T> subarr(std::vector<T> vec, int start, int end);

    #include "stringfunctions.tcc"
}

#endif // !STRING_FUNCTIONS_HPP