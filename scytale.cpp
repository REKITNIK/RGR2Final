#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "scytale.h"
#include "file.h"

using namespace std;

namespace {
    const int DEFAULT_ROWS = 32;
    const int BLOCK_SIZE = 1024;
}


std::vector<uint8_t> encrypt_scytale_block(const std::vector<uint8_t>& block, int rows = DEFAULT_ROWS) {
    int cols = (block.size() + rows - 1) / rows;
    std::vector<uint8_t> cipher;
    cipher.reserve(block.size());
    for (int col = 0; col < cols; ++col) {
        for (int row = 0; row < rows; ++row) {
            int idx = row * cols + col;
            if (idx < block.size()) {
                cipher.push_back(block[idx]);
            }
        }
    }
    return cipher;
}

std::vector<uint8_t> decrypt_scytale_block(const std::vector<uint8_t>& block, int rows = DEFAULT_ROWS) {
    int cols = (block.size() + rows - 1) / rows;
    std::vector<uint8_t> plain(block.size());
    int k = 0;
    for (int col = 0; col < cols; ++col) {
        for (int row = 0; row < rows; ++row) {
            int idx = row * cols + col;
            if (idx < block.size()) {
                plain[idx] = block[k++];
            }
        }
    }
    return plain;
}

std::vector<uint8_t> scytale_encrypt(const std::vector<uint8_t> & payload) {
    const int block_size = BLOCK_SIZE;
    std::vector<uint8_t> cipher;
    cipher.reserve(payload.size());

    for (auto it = payload.cbegin(); it != payload.cend(); ) {
        std::vector<uint8_t> block;
        block.reserve(block_size);

        for (size_t i = 0; i < block_size && it != payload.cend(); ++i, ++it) {
            block.push_back(*it);
        }
        auto encrypted_block = encrypt_scytale_block(block);
        cipher.insert(cipher.end(), encrypted_block.begin(), encrypted_block.end());
    }
    return cipher;
}


std::vector<uint8_t> scytale_encrypt(const std::vector<uint8_t> & payload, const std::string & password) {
    auto pass_block = encrypt_scytale_block(std::vector<uint8_t>(password.begin(), password.end()), 4);
    auto cipher = scytale_encrypt(payload);

    std::vector<uint8_t> result;
    for (char c : pass_block) {
        result.push_back(static_cast<uint8_t>(c));
    }
    result.push_back('\n');
    result.insert(result.end(), cipher.begin(), cipher.end());
    return result;
}


std::vector<uint8_t> scytale_decrypt(const std::vector<uint8_t> & cipher) {
    const int block_size = BLOCK_SIZE;
    std::vector<uint8_t> payload;
    payload.reserve(cipher.size());

    for (auto it = cipher.cbegin(); it != cipher.cend(); ) {
        std::vector<uint8_t> block;
        block.reserve(block_size);

        for (size_t i = 0; i < block_size && it != cipher.cend(); ++i, ++it) {
            block.push_back(*it);
        }
        auto decrypted_block = decrypt_scytale_block(block);
        payload.insert(payload.end(), decrypted_block.begin(), decrypted_block.end());
    }
    return payload;
}


std::vector<uint8_t> scytale_decrypt(const std::vector<uint8_t> & cipher, const std::string & password) {
    auto it = cipher.cbegin();
    vector<uint8_t> user_password_line;
    for (; it != cipher.cend() && *it != '\n'; ++it) {
        user_password_line.push_back(*it);
    }
    auto user_password = decrypt_scytale_block(user_password_line, 4);
    std::string user_pass_str(user_password.begin(), user_password.end());
    if (user_pass_str != password) {
        throw std::runtime_error("Incorrect password for Scytale decryption");
    }
    std::vector<uint8_t> cipher_body;
    for (++it; it != cipher.cend(); ++it) {
        cipher_body.push_back(*it);
    }
    return scytale_decrypt(cipher_body);
}
