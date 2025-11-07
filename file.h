#pragma once
#include <string>
#include <vector>
#include <cstdint>


std::string read_file(const std::string &file_path);

std::vector<std::string> read_lines(const std::string & path);

std::vector<uint8_t> read_file_binary(const std::string & filename);

void write_file_binary(const std::string & filename, const std::vector<uint8_t> & data);

void write_file(const std::string & filename, const std::vector<uint8_t> & data);
