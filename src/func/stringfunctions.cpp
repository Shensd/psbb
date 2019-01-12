#include "stringfunctions.hpp"

/**
 * splits string by given delimeter
 * 
 * @param str string to be split
 * @param delim delimeter to split by
 * @returns vector of strings split by delimeter
 */
std::vector<std::string> split(std::string str, char delim) {
    std::vector<std::string> lines;
    std::istringstream iss(str);
    std::string line;
    while(std::getline(iss, line, delim)) lines.push_back(line);
    return lines;
}

/**
 * replace given sequence in string with another given sequence
 * 
 * @param str string to replace items in
 * @param from sequence to replace from
 * @param to sequence to replace to
 * @returns modified string
 */
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

/**
 * joins an array of strings with given sequence between them
 * 
 * @param strings array of strings to join together
 * @param bridge sequence to join strings together with
 * @returns joined string
 */
std::string join(std::vector<std::string> strings, std::string bridge) {
    std::string combined = "";
    for(int i = 0; i < strings.size(); i++) {
        combined += strings.at(i);
        if(i < strings.size() - 1) {
            combined += bridge;
        }
    }
    return combined;
}

/**
 * converts a given string to lowercase form
 * 
 * @param str string to be converted
 * @returns string in lowercase form
 */
std::string to_lower(const std::string str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}