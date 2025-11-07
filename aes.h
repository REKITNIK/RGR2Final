#pragma once
#include <vector>


std::vector<uint8_t> aes_encrypt(const std::vector<uint8_t> & payload, const std::string & password);


std::vector<uint8_t> aes_decrypt(const std::vector<uint8_t> & cipher, const std::string & password);