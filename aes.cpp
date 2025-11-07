#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

namespace {
    random_device rd("/dev/random");
    mt19937_64 random_generator { rd() };

    unsigned char s_box[256] = {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, 
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
    };

    unsigned char rcon[256] = {
        0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 
        0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 
        0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
        0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
        0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
        0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
        0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
        0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
        0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
        0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
        0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
        0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
        0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
        0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
        0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
        0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
    };

    unsigned char inv_s_box[256] = {
        0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
        0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
        0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
        0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
        0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
        0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
        0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
        0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
        0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
        0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
        0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
        0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
        0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
        0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
        0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
        0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
    };

    const auto Nk = 4;
    const auto Nb = 4;
    const auto Nr = 10;
}

vector<unsigned char> expand_key(const std::string& key) {
    // Функция для расширения ключа AES-128

    vector<unsigned char> expanded_key((Nr + 1) * Nk * Nb, 0);

    // Копируем исходный ключ (16 байт)
    for (int i = 0; i < 16; ++i) {
        expanded_key[i] = static_cast<unsigned char>(key[i]);
    }

    int bytes_generated = 16;
    int rcon_iter = 1;
    unsigned char temp[4];

    while (bytes_generated < 176) {
        // Копируем последние 4 байта
        for (int i = 0; i < 4; ++i)
            temp[i] = expanded_key[bytes_generated - 4 + i];

        if (bytes_generated % 16 == 0) {
            // Сдвиг влево
            unsigned char t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;
            // S-box
            for (int i = 0; i < 4; ++i)
                temp[i] = s_box[temp[i]];
            // Rcon
            temp[0] ^= rcon[rcon_iter++];
        }

        for (int i = 0; i < 4; ++i) {
            expanded_key[bytes_generated] = expanded_key[bytes_generated - 16] ^ temp[i];
            ++bytes_generated;
        }
    }

    return expanded_key;
}

void sub_bytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            state[i][j] = s_box[state[i][j]];
}

void shift_rows(unsigned char state[4][4]) {
    unsigned char tmp;
    // 2 row
    tmp = state[1][0];
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = tmp;
    // 3 row
    tmp = state[2][0];
    unsigned char tmp2 = state[2][1];
    state[2][0] = state[2][2];
    state[2][1] = state[2][3];
    state[2][2] = tmp;
    state[2][3] = tmp2;
    // 4 row
    tmp = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = tmp;
}

unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    for (int i = 0; i < 8; ++i) {
        if (b & 1) p ^= a;
        bool hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

void mix_columns(unsigned char state[4][4]) {
    unsigned char tmp[4];
    for (int i = 0; i < 4; ++i) {
        tmp[0] = gmul(0x02, state[0][i]) ^ gmul(0x03, state[1][i]) ^ state[2][i] ^ state[3][i];
        tmp[1] = state[0][i] ^ gmul(0x02, state[1][i]) ^ gmul(0x03, state[2][i]) ^ state[3][i];
        tmp[2] = state[0][i] ^ state[1][i] ^ gmul(0x02, state[2][i]) ^ gmul(0x03, state[3][i]);
        tmp[3] = gmul(0x03, state[0][i]) ^ state[1][i] ^ state[2][i] ^ gmul(0x02, state[3][i]);
        for (int j = 0; j < 4; ++j)
            state[j][i] = tmp[j];
    }
}

void add_round_key(unsigned char state[4][4], const vector<unsigned char>& round_key, int round) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            state[j][i] ^= round_key[round * 16 + i * 4 + j];
}

vector<unsigned char> encrypt_aes128(const string& input, const vector<unsigned char>& expanded_key) {
    vector<unsigned char> cyphertext;

    for (size_t offset = 0; offset < input.size(); offset += 16) {
        unsigned char state[4][4];
        // Заполняем state по столбцам
        for (int i = 0; i < 16; ++i)
            state[i % 4][i / 4] = static_cast<unsigned char>(input[offset + i]);

        add_round_key(state, expanded_key, 0);

        for (int round = 1; round < 10; ++round) {
            sub_bytes(state);
            shift_rows(state);
            mix_columns(state);
            add_round_key(state, expanded_key, round);
        }

        sub_bytes(state);
        shift_rows(state);
        add_round_key(state, expanded_key, 10);

        for (int i = 0; i < 16; ++i) {
            cyphertext.push_back(state[i % 4][i / 4]);
        }
    }
    return cyphertext;
}


void inv_shift_rows(unsigned char state[4][4]) {
    unsigned char tmp;
    // 2 row
    tmp = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = state[1][0];
    state[1][0] = tmp;
    // 3 row
    tmp = state[2][0];
    state[2][0] = state[2][2];
    state[2][2] = tmp;
    tmp = state[2][1];
    state[2][1] = state[2][3];
    state[2][3] = tmp;
    // 4 row
    tmp = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = tmp;
}

void inv_sub_bytes(unsigned char state[4][4]) {
    // обратная s-box
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            state[i][j] = inv_s_box[state[i][j]];
}

void inv_mix_columns(unsigned char state[4][4]) {
    unsigned char tmp[4];
    for (int i = 0; i < 4; ++i) {
        tmp[0] = gmul(0x0e, state[0][i]) ^ gmul(0x0b, state[1][i]) ^ gmul(0x0d, state[2][i]) ^ gmul(0x09, state[3][i]);
        tmp[1] = gmul(0x09, state[0][i]) ^ gmul(0x0e, state[1][i]) ^ gmul(0x0b, state[2][i]) ^ gmul(0x0d, state[3][i]);
        tmp[2] = gmul(0x0d, state[0][i]) ^ gmul(0x09, state[1][i]) ^ gmul(0x0e, state[2][i]) ^ gmul(0x0b, state[3][i]);
        tmp[3] = gmul(0x0b, state[0][i]) ^ gmul(0x0d, state[1][i]) ^ gmul(0x09, state[2][i]) ^ gmul(0x0e, state[3][i]);
        for (int j = 0; j < 4; ++j)
            state[j][i] = tmp[j];
    }
}

string decrypt_aes128(const vector<unsigned char>& cyphertext, const vector<unsigned char>& expanded_key) {
    string result;

    for (size_t offset = 0; offset < cyphertext.size(); offset += 16) {
        unsigned char state[4][4];
        // Заполняем state по столбцам
        for (int i = 0; i < 16; ++i)
            state[i % 4][i / 4] = cyphertext[offset + i];

        add_round_key(state, expanded_key, 10);

        for (int round = 9; round >= 1; --round) {
            inv_shift_rows(state);
            inv_sub_bytes(state);
            add_round_key(state, expanded_key, round);
            inv_mix_columns(state);
        }

        inv_shift_rows(state);
        inv_sub_bytes(state);
        add_round_key(state, expanded_key, 0);

        // Собираем результат
        for (int i = 0; i < 16; ++i)
            result += static_cast<char>(state[i % 4][i / 4]);
    }
    return result;
}

vector<unsigned char> aes128_encrypt_block(const unsigned char in[16], const vector<unsigned char>& expanded_key) {
    unsigned char state[4][4];
    vector<unsigned char> out_block(16);

    // Заполняем state по столбцам
    for (int i = 0; i < 16; ++i) {
        state[i % 4][i / 4] = in[i];
    }

    add_round_key(state, expanded_key, 0);

    for (int round = 1; round < 10; ++round) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, expanded_key, round);
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, expanded_key, 10);

    // Собираем результат
    for (int i = 0; i < 16; ++i)
        out_block[i] = state[i % 4][i / 4];

    return out_block;
}

vector<uint8_t> aes128_ofb(const vector<uint8_t> & input, const vector<unsigned char>& expanded_key, vector<unsigned char> iv) {
    vector<uint8_t> output;
    auto blocks = input.size() / 16;
    vector<unsigned char> prev = iv;

    for (int b = 0; b < blocks; ++b) {
        unsigned char state[4][4];
        for (int i = 0; i < 16; ++i) {
            state[i % 4][i / 4] = prev[i];
        }

        // Получаем следующий keystream-блок
        auto keystream = aes128_encrypt_block(prev.data(), expanded_key);

        // XOR с plaintext/cyphertext
        for (int i = 0; i < 16; ++i) {
            unsigned char in_byte = (b * 16 + i < input.size()) ? static_cast<unsigned char>(input[b * 16 + i]) : 0;
            output.push_back(in_byte ^ keystream[i]);
        }
        prev = keystream;
    }
    return output;
}

vector<uint8_t> aes128_ofb_decrypt(const vector<uint8_t> & cyphertext, const vector<unsigned char>& expanded_key, vector<unsigned char> iv) {
    vector<uint8_t> output;
    size_t blocks = (cyphertext.size() + 15) / 16;
    vector<unsigned char> prev = iv;

    for (size_t b = 0; b < blocks; ++b) {
        auto keystream = aes128_encrypt_block(prev.data(), expanded_key);

        for (int i = 0; i < 16; ++i) {
            unsigned char c = (b * 16 + i < cyphertext.size()) ? cyphertext[b * 16 + i] : 0;
            output.push_back(c ^ keystream[i]);
        }
        prev = keystream;
    }
    return output;
}


std::string to_hex(const std::vector<uint8_t> & data) {
    std::ostringstream oss;
    for (auto byte : data) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

std::string to_text(const std::vector<uint8_t> & data) {
    std::ostringstream oss;
    for (auto byte : data) {
        oss << static_cast<char>(byte);
    }
    return oss.str();
}


void write_data_size(std::vector<uint8_t> & data, uint32_t size) {
    data.push_back((size >> 24) & 0xFF);
    data.push_back((size >> 16) & 0xFF);
    data.push_back((size >> 8) & 0xFF);
    data.push_back(size & 0xFF);
}

uint32_t read_data_size(std::vector<uint8_t>::const_iterator it) {
    uint32_t size = 0;
    size |= static_cast<uint32_t>(*(it)) << 24;
    size |= static_cast<uint32_t>(*(++it)) << 16;
    size |= static_cast<uint32_t>(*(++it)) << 8;
    size |= static_cast<uint32_t>(*(++it));
    return size;
}

std::vector<uint8_t> aes_encrypt(const std::vector<uint8_t> & payload, const std::string & password) {
    const vector<unsigned char> iv(16, 0x00);
    const auto expanded_key = expand_key(password);

    auto password_cipher = aes128_ofb(std::vector<uint8_t>(password.begin(), password.end()), expanded_key, iv);

    std::vector<uint8_t> result;
    for (const auto & c: password_cipher) {
        result.push_back(c);
    }

    const uint32_t real_size = payload.size();
    write_data_size(result, real_size);

    const auto extra_bytes = 16 - (real_size % 16);
    std::vector<uint8_t> padded_payload = payload;
    for (uint32_t i = 0; i < extra_bytes; ++i) {
        padded_payload.push_back(0x00);
    }

    auto cipher = aes128_ofb(padded_payload, expanded_key, iv);
    for (const auto & c: cipher) {
        result.push_back(c);
    }
    return result;
}


std::vector<uint8_t> aes_decrypt(const std::vector<uint8_t> & cipher, const std::string & password) {
    vector<unsigned char> iv(16, 0x00);

    if (cipher.size() < (16 + 4)) {
        throw std::runtime_error("AES decryption error: data is corrupted");
    }
    const auto password_buffer = vector<uint8_t>(cipher.cbegin(), cipher.cbegin() + 16);
    const auto expanded_key = expand_key(password);
    const auto user_password_buffer = aes128_ofb_decrypt(password_buffer, expanded_key, iv);

    std::string user_password(user_password_buffer.begin(), user_password_buffer.end());

    if (user_password != password) {
        throw std::runtime_error("AES decryption error: wrong password");
    }

    uint32_t real_size = read_data_size(cipher.cbegin() + 16);

    std::vector<uint8_t> cipher_body;
    for (auto it = cipher.cbegin() + 16 + 4; it != cipher.cend(); ++it) {
        cipher_body.push_back(*it);
    }
    auto body = aes128_ofb_decrypt(cipher_body, expanded_key, iv);

    if (body.size() > real_size) {
        body.resize(real_size);
    }
    return body;
}