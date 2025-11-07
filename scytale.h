#pragma once
#include <string>
#include <vector>
#include <cstdint>


std::vector<uint8_t> scytale_encrypt(const std::vector<uint8_t> & payload, const std::string & password);


std::vector<uint8_t> scytale_decrypt(const std::vector<uint8_t> & cipher, const std::string & password);
