#include <iostream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <cstdint>
#include <fstream>
#include <sstream>
#include "morse.h"
#include "scytale.h"
#include "file.h"
#include "prompt.h"
#include "aes.h"

using namespace std;
namespace fs = std::filesystem;


namespace {

    std::string EXT_MORSE = ".morse";
    std::string EXT_SCYTALE = ".scytale";
    std::string EXT_BLOCK = ".block";

    bool check_file_extension(const std::string &filename, const std::string &extension)
    {
        fs::path filepath(filename);
        return filepath.extension() == extension;
    }

}

void show_usage(int argc, char** argv) {
    std::cerr << "Usage:" << endl
        << "  " << argv[0] << " e(ncrypt) <file_path> <1(Morse), 2(Block), 3(Scytale)>" << endl
        << "  " << argv[0] << " d(ecrypt) <file_path>" << endl;
}

std::string encrypted_file_path(const std::string & original_path, const std::string & extension) {
    fs::path filepath(original_path);
    return filepath.parent_path() / (filepath.filename().string() + extension);
}

std::string decrypted_file_path(const std::string & encrypted_path) {
    fs::path filepath(encrypted_path);
    return filepath.parent_path() / ("decrypted_" + filepath.stem().string());
}


int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RUS");
    if (argc < 3) {
        show_usage(argc, argv);
        return 1;
    }

    const char mode = argv[1][0];
    if (mode != 'e' && mode != 'd') {
        std::cerr << "Unknown mode: " << mode << ". Use 'e' for encrypt or 'd' for decrypt." << endl;
        show_usage(argc, argv);
        return 1;
    }

    const string file_path = argv[2];
    if (!fs::exists(file_path)) {
        std::cerr << "File not found: " << file_path << endl;
        show_usage(argc, argv);
        return 1;
    }

    const auto password = get_password();

    if (mode == 'e') {
        if (argc < 4) {
            std::cerr << "Missing variant for encryption. Use 1 for Morse, 2 for Block, or 3 for Scytale." << endl;
            show_usage(argc, argv);
            return 1;
        }

        int variant = std::atoi(argv[3]);
        auto payload = read_file_binary(file_path);

        switch (variant) {
            case 1: {
                const auto cipher = morse_encrypt(payload, password);
                auto encrypted_path = encrypted_file_path(file_path, EXT_MORSE);
                write_file_binary(encrypted_path, cipher);
                std::cout << "Morse encryption done: " << encrypted_path << std::endl;
                return 0;
            }
            case 2: {
                const auto cipher = aes_encrypt(payload, password);
                auto encrypted_path = encrypted_file_path(file_path, EXT_BLOCK);
                write_file_binary(encrypted_path, cipher);
                std::cout << "Block encryption done: " << encrypted_path << std::endl;
                return 0;
            }
            case 3: {
                const auto cipher = scytale_encrypt(payload, password);
                auto encrypted_path = encrypted_file_path(file_path, EXT_SCYTALE);
                write_file_binary(encrypted_path, cipher);
                std::cout << "Scytale encryption done: " << encrypted_path << std::endl;
                return 0;
            }
            default:
                std::cerr << "Unknown variant: " << variant << ". Use 1 for Morse, 2 for Block, or 3 for Scytale.\n";
                return 1;
        }
    } 
    else if (mode == 'd') {
        auto cipher = read_file_binary(file_path);
        auto decrypted_path = decrypted_file_path(file_path);
        
        try {
            if (check_file_extension(file_path, EXT_MORSE)) {
                auto payload = morse_decrypt(cipher, password);
                write_file_binary(decrypted_path, payload);
                std::cout << "Morse decryption done: " << decrypted_path<< std::endl;
                return 0;
            } else if (check_file_extension(file_path, EXT_SCYTALE)) {
                auto payload = scytale_decrypt(cipher, password);
                write_file_binary(decrypted_path, payload);
                std::cout << "Scytale decryption done: " << decrypted_path << std::endl;
                return 0;
            } else if (check_file_extension(file_path, EXT_BLOCK)) {
                auto payload = aes_decrypt(cipher, password);
                write_file_binary(decrypted_path, payload);
                std::cout << "AES decryption done: " << decrypted_path << std::endl;
                return 0;
            } else {
                std::cerr << "Unknown file extension. Expected .morse or .scytale\n";
                return 1;
            }
        } catch (const std::exception & ex) {
            std::cerr << ex.what() << std::endl;
            return 1;
        }
    } 
    else {
        std::cerr << "Unknown mode: " << mode << ". Use 'e' for encrypt or 'd' for decrypt.\n";
        return 1;
    }
    
    return 0;
}
