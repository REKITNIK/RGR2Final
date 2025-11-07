#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include "file.h"
#include "morse.h"
#include "prompt.h"
#include <cwchar>

using namespace std;


namespace {
    const std::unordered_map<char, std::string> MORSE_TABLE = {
        {'A', "~.-"},    {'B', "~-..."},  {'C', "~-.-."}, {'D', "~-.."},   {'E', "~."},
        {'F', "~..-."},  {'G', "~--."},   {'H', "~...."}, {'I', "~.."},    {'J', "~.---"},
        {'K', "~-.-"},   {'L', "~.-.."},  {'M', "~--"},   {'N', "~-."},    {'O', "~---"},
        {'P', "~.--."},  {'Q', "~--.-"},  {'R', "~.-."},  {'S', "~..."},   {'T', "~-"},
        {'U', "~..-"},   {'V', "~...-"},  {'W', "~.--"},  {'X', "~-..-"},  {'Y', "~-.--"},
        {'Z', "~--.."},
        {'0', "-----"}, {'1', ".----"},{'2', "..---"}, {'3', "...--"},
        {'4', "....-"}, {'5', "....."}, {'6', "-...."},{'7', "--..."}, {'8', "---.."},
        {'9', "----."},
        {'a', ".-"},    {'b', "-..."},  {'c', "-.-."}, {'d', "-.."},   {'e', "~~."},
        {'f', "..-."},  {'g', "--."},   {'h', "...."}, {'i', ".."},    {'j', ".---"},
        {'k', "-.-"},   {'l', ".-.."},  {'m', "--"},   {'n', "-."},    {'o', "---"},
        {'p', ".--."},  {'q', "--.-"},  {'r', ".-."},  {'s', "..."},   {'t', "~~-"},
        {'u', "..-"},   {'v', "...-"},  {'w', ".--"},  {'x', "-..-"},  {'y', "-.--"},
        {'z', "--.."},
        {':', "---..."}, {'.', ".-.-.-"}, {',', "--..--"}, {'?', "..--.."}, {'!', "-.-.--"},
        {';', "-.-.-."}, {'-', "-....-"}, {')', "-.--.-"}, {'(', "-.--.-"},
        {L'А', ".-"},    {L'Б', "-..."},  {L'В', ".--"},   {L'Г', "--."},   {L'Д', "-.."},
        {L'Е', "."},     {L'Ж', "...-"},  {L'З', "--.."},  {L'И', ".."},    {L'Й', ".---"},
        {L'К', "-.-"},   {L'Л', ".-.."},  {L'М', "--"},    {L'Н', "-."},    {L'О', "---"},
        {L'П', ".--."},  {L'Р', ".-."},   {L'С', "..."},   {L'Т', "-"},     {L'У', "..-"},
        {L'Ф', "..-."},  {L'Х', "...."},  {L'Ц', "-.-."},  {L'Ч', "---."},  {L'Ш', "----"},
        {L'Щ', "--.-"},  {L'Ъ', "--.--"}, {L'Ы', "-.--"},  {L'Ь', "-..-"},  {L'Э', "..-.."},
        {L'Ю', "..--"},  {L'Я', ".-.-"},
        {L'а', ".-"},    {L'б', "-..."},  {L'в', ".--"},   {L'г', "--."},   {L'д', "-.."},
        {L'е', "."},     {L'ж', "...-"},  {L'з', "--.."},  {L'и', ".."},    {L'й', ".---"},
        {L'к', "-.-"},   {L'л', ".-.."},  {L'м', "--"},    {L'н', "-."},    {L'о', "---"},
        {L'п', ".--."},  {L'р', ".-."},   {L'с', "..."},   {L'т', "-"},     {L'у', "..-"},
        {L'ф', "..-."},  {L'х', "...."},  {L'ц', "-.-."},  {L'ч', "---."},  {L'ш', "----"},
        {L'щ', "--.-"},  {L'ъ', "--.--"}, {L'ы', "-.--"},  {L'ь', "-..-"},  {L'э', "..-.."},
        {L'ю', "..--"},  {L'я', ".-.-"},
    };

    const std::unordered_map<std::string, char> MORSE_TABLE_REV = []{
        std::unordered_map<std::string, char> m;
        for (const auto & p : MORSE_TABLE) {
            m[p.second] = p.first;
        }
        return m;
    }();
}


std::vector<uint8_t> morse_encrypt(const std::vector<uint8_t> & payload) {
    std::vector<std::string> buffer;
    for (char c : payload) {
        auto it = MORSE_TABLE.find(c);
        if (it != MORSE_TABLE.end()) {
            buffer.push_back(it->second);
        } else {
            buffer.push_back(std::string(1, c));
        }
    }

    std::vector<uint8_t> cipher;
    for (const auto & code : buffer) {
        for (char c : code) {
            cipher.push_back(static_cast<uint8_t>(c));
        }
        cipher.push_back('\n');
    }
    return cipher;
}


string encrypt_password_line(const std::string & line) {
    std::ostringstream oss;
    for (char c : line) {
        auto it = MORSE_TABLE.find(c);
        if (it != MORSE_TABLE.end()) {
            oss << it->second << " ";
        } else {
            oss << c << " "; // If character not in Morse table, keep it as is
        }
    }
    return oss.str();
}


string decrypt_password_line(const std::string & line) {
    std::istringstream iss(line);
    std::ostringstream oss;
    std::string code;

    while (iss >> code) {
        auto it = MORSE_TABLE_REV.find(code);
        if (it != MORSE_TABLE_REV.end()) {
            oss << it->second;
        } else {
            oss << code; // If code not in Morse table, keep it as is
        }
    }
    return oss.str();
}


std::vector<uint8_t> morse_encrypt(const std::vector<uint8_t> & payload, const std::string & password) {
    const auto encrypted_password = encrypt_password_line(password);
    auto cipher = morse_encrypt(payload);

    std::vector<uint8_t> result;
    result.reserve(cipher.size() + encrypted_password.size() + 1);
    for (char c : encrypted_password) {
        result.push_back(static_cast<uint8_t>(c));
    }
    result.push_back('\n');
    result.insert(result.end(), cipher.begin(), cipher.end());
    return result;
}


std::vector<uint8_t> morse_decrypt(const std::vector<uint8_t> & cipher, const std::string & password) {
    std::string user_password_line;
    auto it = cipher.cbegin();
    for (; it != cipher.cend() && *it != '\n'; ++it) {
        user_password_line.push_back(static_cast<char>(*it));
    }
    const auto user_password = decrypt_password_line(user_password_line);
    if (user_password != password) {
        throw std::runtime_error("Decryption failed: Invalid password");
    }

    std::vector<std::string> buffer;

    for (++it; it != cipher.cend(); ++it) {
        std::string token;

        while (it != cipher.cend() && *it != '\n') {
            token.push_back(static_cast<char>(*it));
            ++it;
        }
        if (!token.empty()) {
            auto letter_it = MORSE_TABLE_REV.find(token);
            if (letter_it != MORSE_TABLE_REV.end()) {
                buffer.push_back(std::string(1, letter_it->second));
            } else {
                buffer.push_back(token);
            }
        } else if (*it == '\n') {
            buffer.push_back(std::string(1, '\n'));
            ++it;
        }
    }

    std::vector<uint8_t> payload;
    for (const auto & str : buffer) {
        for (char c : str) {
            payload.push_back(static_cast<uint8_t>(c));
        }
    }
    return payload;
}
