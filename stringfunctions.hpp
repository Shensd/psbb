#ifndef STR_FUNCS_H
#define STR_FUNCS_H

#include <string>
#include <vector>

std::vector<std::string> split(std::string str, char delim) {
    std::vector<std::string> lines;
    std::istringstream iss(str);
    std::string line;
    while(std::getline(iss, line, delim)) lines.push_back(line);
    return lines;
}

std::string replace(std::string str, std::string from, std::string to) {
    std::string replaced = str;

    int position = replaced.find(from);
    int last_position = 0;
    while(position != std::string::npos) {
        replaced = replaced.substr(0, position) + 
                   to + 
                   replaced.substr(position + from.length(), replaced.length());
        last_position = position;
        position = replaced.find(from);
    }

    return replaced;
}

#endif // !STR_FUNCS_H