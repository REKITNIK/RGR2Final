#pragma once
#include <string>
#include <vector>


std::vector<uint8_t> morse_encrypt(const std::vector<uint8_t> & payload, const std::string & password);


std::vector<uint8_t> morse_decrypt(const std::vector<uint8_t> & cipher, const std::string & password);