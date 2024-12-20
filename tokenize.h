#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream> // For std::stringstream

std::vector<std::string> tokenise(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}