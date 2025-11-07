#include "file.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


std::string read_file(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Cannot open file: " << filePath << std::endl;
        std::exit(1);
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}


std::vector<std::string> read_lines(const std::string& path) {
    std::ifstream in_file(path);
    if (!in_file.is_open()) {
        std::cerr << "Cannot open file for reading: " << path << std::endl;
        std::exit(1);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in_file, line)) {
        lines.push_back(line);
    }
    in_file.close();
    return lines;
}


std::vector<uint8_t> read_file_binary(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error(std::string("Cannot open input file: ") + filename);
    }
    return std::vector<uint8_t>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}

void write_file_binary(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error(std::string("Cannot open output file: ") + filename);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}


void write_file(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error(std::string("Cannot open output file: ") + filename);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

