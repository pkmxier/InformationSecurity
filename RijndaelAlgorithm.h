//
// Created by pkmixer on 05.04.2021.
//

#ifndef INFORMATIONSECURITYPOLYNOMIAL_RIJNDAELALGORITHM_H
#define INFORMATIONSECURITYPOLYNOMIAL_RIJNDAELALGORITHM_H
#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include "Utils.h"

class RijndaelAlgorithm {
private:
    int keySize;
    int rounds;

    std::vector<uint8_t> sBox, inversedSBox;
    std::vector<uint8_t> rCon;

    static void generateRCon(std::vector<uint8_t> &rCon);
    static void generateSBoxes(std::vector<uint8_t> &SBox, std::vector<uint8_t> &InversedSBox);
    void KeysExpansion(const std::vector<uint8_t> &key, std::vector<uint8_t> roundKey);
    void AddRoundKey(int round, std::vector<std::vector<uint8_t>> &state, const std::vector<uint8_t> &roundKey);
    void SubBytes(std::vector<std::vector<uint8_t>> &state);
    void InversedSubBytes(std::vector<std::vector<uint8_t>> &state);
    std::vector<std::vector<uint8_t>> ShiftRows(const std::vector<std::vector<uint8_t>> &state);
    std::vector<std::vector<uint8_t>> InversedShiftRows(const std::vector<std::vector<uint8_t>> &state);
    void MixColumns(std::vector<std::vector<uint8_t>> &state);
    void InversedMixColumns(std::vector<std::vector<uint8_t>> &state);

    std::vector<uint8_t> EncryptBytes(const std::vector<uint8_t> &key, const std::vector<uint8_t> &text);
    std::vector<uint8_t> EncryptBlock(const std::vector<uint8_t> &key, const std::vector<uint8_t> &text);
    std::vector<uint8_t> DecryptBytes(const std::vector<uint8_t> &key, const std::vector<uint8_t> &text);
    std::vector<uint8_t> DecryptBlock(const std::vector<uint8_t> &key, const std::vector<uint8_t> &text);

public:
    RijndaelAlgorithm(int keySize);

    std::string Encrypt(const std::string &key, const std::string &text);
    std::string Decrypt(const std::string &key, const std::string &text, int cutoff);
};


#endif //INFORMATIONSECURITYPOLYNOMIAL_RIJNDAELALGORITHM_H
