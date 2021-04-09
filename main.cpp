#include <iostream>
#include <fstream>
#include <cassert>
#include <random>
#include "RijndaelAlgorithm.h"

std::string readFromFile(const std::string &path) {
    std::ifstream input_stream(path);

    return std::string(std::istreambuf_iterator<char>(input_stream),
                       std::istreambuf_iterator<char>());
}

std::string generateRandomKey(int keyLength) {
    std::random_device rd;
    std::mt19937 generator(rd());

    std::string key;

    for (int i = 0; i < keyLength; ++i) {
        key += char(generator() % 255);
    }

    return key;
}

int main() {
    int keyLength = 128;
    std::string text = readFromFile("../input.txt");
    std::string key = generateRandomKey(keyLength / 8);

    RijndaelAlgorithm rijndael(keyLength);

    std::string encrypted = rijndael.Encrypt(key, text);
    std::string decrypted = rijndael.Decrypt(key, encrypted, text.size());

    std::cout << text << std::endl;
    std::cout << decrypted << std::endl;

    assert(text.size() == decrypted.size());
    for (int i = 0; i < text.size(); ++i) {
        assert(text[i] == decrypted[i]);
    }
    std::cout << "OK" << std::endl;
}
