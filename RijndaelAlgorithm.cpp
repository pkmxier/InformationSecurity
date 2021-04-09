//
// Created by pkmixer on 05.04.2021.
//

#include "RijndaelAlgorithm.h"
#include <cassert>

RijndaelAlgorithm::RijndaelAlgorithm(int keySize) {
    this->keySize = keySize / 32;
    this->rounds = this->keySize + 6;

    this->sBox = std::vector<uint8_t>(256);
    this->inversedSBox = std::vector<uint8_t>(256);
    this->rCon = std::vector<uint8_t>(256);

    generateSBoxes(sBox, inversedSBox);
    generateRCon(rCon );
}

void RijndaelAlgorithm::generateRCon(std::vector<uint8_t> &rCon) {
    uint8_t x = 0x01;

    rCon[0] = 0x8d; // todo: remove
    rCon[1] = x;

    for (int i = 2; i < rCon.size(); ++i) {
        rCon[i] = Multiplication(rCon[i - 1], 2);
    }
}

void RijndaelAlgorithm::generateSBoxes(std::vector<uint8_t> &SBox, std::vector<uint8_t> &InversedSBox) {
    uint8_t f = 0x63;

    SBox[0] = f;
    InversedSBox[f] = 0;

    for (int i = 1; i < 256; ++i) {
        uint8_t b = InverseOf(i);
        SBox[i] = b ^ rotl(b, 1) ^ rotl(b, 2) ^ rotl(b, 3) ^ rotl(b, 4) ^ 0x63;
        InversedSBox[SBox[i]] = i;
    }
}

std::vector<uint8_t> RijndaelAlgorithm::DecryptBytes(const std::vector<uint8_t> &key, const std::vector<uint8_t> &text) {
    std::vector<uint8_t> result;

    int i = 0;

    std::vector<uint8_t> block(key.size());
    std::vector<uint8_t> blockResult;

    while (i < text.size()) {
        int j = 0;
        while (j < key.size() && i < text.size()) {
            block[j] = text[i];
            ++i; ++j;
        }

        blockResult = DecryptBlock(key, block);

        for (int k = 0; k < blockResult.size(); ++k) {
            result.push_back(blockResult[k]);
        }
    }

    return result;
}

std::vector<uint8_t> RijndaelAlgorithm::DecryptBlock(const std::vector<uint8_t> &key, const std::vector<uint8_t> &text) {
    std::vector<std::vector<uint8_t>> state(4, std::vector<uint8_t>(keySize));
    std::vector<uint8_t> result(text.size());

    for (int i = 0; i < keySize; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[j][i] = text[4 * i + j];
        }
    }

    AddRoundKey(rounds, state, key);

    for (int round = rounds - 1; round > 0; --round) {
        state = InversedShiftRows(state);
        InversedSubBytes(state);
        AddRoundKey(round, state, key);
        InversedMixColumns(state);
    }

    state = InversedShiftRows(state);
    InversedSubBytes(state);
    AddRoundKey(0, state, key);

    for (int i = 0; i < keySize; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[4 * i + j] = state[j][i];
        }
    }

    return result;
}

std::vector<uint8_t> RijndaelAlgorithm::EncryptBytes(const std::vector<uint8_t> &key, const std::vector<uint8_t> &text) {
    std::vector<uint8_t> result;

    int i = 0;

    std::vector<uint8_t> block(key.size());
    std::vector<uint8_t> blockResult;

    while (i < text.size()) {
        int j = 0;
        while (j < key.size() && i < text.size()) {
            block[j] = text[i];
            ++i; ++j;
        }

        if (j < key.size()) {
            for (int k = j; k < key.size(); ++k) {
                block[k] = 0;
            }
        }

        blockResult = EncryptBlock(key, block);

        for (int k = 0; k < blockResult.size(); ++k) {
            result.push_back(blockResult[k]);
        }
    }

    return result;
}

std::vector<uint8_t> RijndaelAlgorithm::EncryptBlock(const std::vector<uint8_t> &key, const std::vector<uint8_t> &text) {
    int round = 0;

    std::vector<uint8_t> result(text.size());

    std::vector<std::vector<uint8_t>> state(4, std::vector<uint8_t>(keySize));
    for (int i = 0; i < keySize; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[j][i] = text[4 * i + j];
        }
    }

    AddRoundKey(0, state, key);

    for (int round = 1; round < rounds; ++round) {
        SubBytes(state);
        state = ShiftRows(state);
        MixColumns(state);
        AddRoundKey(round, state, key);
    }

    SubBytes(state);
    state = ShiftRows(state);
    AddRoundKey(rounds, state, key);

    for (int i = 0; i < keySize; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[4 * i + j] = state[j][i];
        }
    }

    return result;
}

void RijndaelAlgorithm::KeysExpansion(const std::vector<uint8_t> &key, std::vector<uint8_t> roundKey) {
    int i;
    uint8_t tempBytes[4], temp;

    for (i = 0; i < keySize; ++i) {
        for (int j = 0; j < 4; ++j) {
            roundKey[4 * i + j] = key[4 * i + j];
        }
    }

    while (i < (keySize * (rounds + 1))) {
        for (int j = 0; j < 4; ++j) {
            tempBytes[j] = roundKey[(i - 1) * 4 + j];
        }

        if (i % keySize == 0) {
            temp = tempBytes[0];
            tempBytes[0] = tempBytes[1];
            tempBytes[1] = tempBytes[2];
            tempBytes[2] = tempBytes[3];
            tempBytes[3] = temp;

            tempBytes[0] = sBox[tempBytes[0]];
            tempBytes[1] = sBox[tempBytes[1]];
            tempBytes[2] = sBox[tempBytes[2]];
            tempBytes[3] = sBox[tempBytes[3]];

            tempBytes[0] = tempBytes[0] ^ rCon[i / keySize];
        } else if (keySize > 6 && i % keySize == 4) {
            tempBytes[0] = sBox[tempBytes[0]];
            tempBytes[1] = sBox[tempBytes[1]];
            tempBytes[2] = sBox[tempBytes[2]];
            tempBytes[3] = sBox[tempBytes[3]];
        }

        roundKey[4 * i] = roundKey[(i - keySize) * 4] ^ tempBytes[0];
        roundKey[4 * i + 1] = roundKey[(i - keySize) * 4 + 1] ^ tempBytes[1];
        roundKey[4 * i + 2] = roundKey[(i - keySize) * 4 + 2] ^ tempBytes[2];
        roundKey[4 * i + 3] = roundKey[(i - keySize) * 4 + 3] ^ tempBytes[3];
        ++i;
    }
}

void RijndaelAlgorithm::AddRoundKey(int round, std::vector<std::vector<uint8_t>> &state, const std::vector<uint8_t> &roundKey) {
    for (int i = 0; i < keySize; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[j][i] ^= roundKey[keySize * (4 * round + i) + j];
        }
    }
}

void RijndaelAlgorithm::SubBytes(std::vector<std::vector<uint8_t>> &state) {
    for (int i = 0; i < keySize; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[j][i] = sBox[state[j][i]];
        }
    }
}

void RijndaelAlgorithm::InversedSubBytes(std::vector<std::vector<uint8_t>> &state) {
    for (int i = 0; i < keySize; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[j][i] = inversedSBox[state[j][i]];
        }
    }
}

std::vector<std::vector<uint8_t>> RijndaelAlgorithm::ShiftRows(const std::vector<std::vector<uint8_t>> &state) {
    std::vector<std::vector<uint8_t>> result(state.size(), std::vector<uint8_t>(state[0].size()));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < keySize; ++j) {
            int index = (i + j) % keySize;
            result[i][j] = state[i][index];
        }
    }

    return result;
}

std::vector<std::vector<uint8_t>> RijndaelAlgorithm::InversedShiftRows(const std::vector<std::vector<uint8_t>> &state) {
    std::vector<std::vector<uint8_t>> result(state.size(), std::vector<uint8_t>(state[0].size()));
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < keySize; ++j) {
            int index = (i + j) % keySize;
            result[i][index] = state[i][j];
        }
    }

    return result;
}

std::string RijndaelAlgorithm::Encrypt(const std::string &key, const std::string &text) {
    assert(key.size() == keySize * 4);

    std::vector<uint8_t> keyBytes(key.size());
    for (int i = 0; i < keyBytes.size(); ++i) {
        keyBytes[i] = (uint8_t) key[i];
    }

    int extendedSize = (text.size() / key.size()) * key.size() +
            ((text.size() % key.size() == 0) ? 0 : key.size());

    std::vector<uint8_t> textBytes(extendedSize, 0);

    for (int i = 0; i < text.size(); ++i) {
        textBytes[i] = (uint8_t) text[i];
    }

    std::vector<uint8_t> encryptedBytes = EncryptBytes(keyBytes, textBytes);

    std::string result(encryptedBytes.size(), 0);
    for (int i = 0; i < result.size(); ++i) {
        result[i] = (char) encryptedBytes[i];
    }

    return result;
}

std::string RijndaelAlgorithm::Decrypt(const std::string &key, const std::string &text, int cutoff) {
    assert(key.size() == keySize * 4);

    std::vector<uint8_t> keyBytes(key.size());
    for (int i = 0; i < keyBytes.size(); ++i) {
        keyBytes[i] = (uint8_t) key[i];
    }

    std::vector<uint8_t> textBytes(text.size(), 0);

    for (int i = 0; i < text.size(); ++i) {
        textBytes[i] = (uint8_t) text[i];
    }

    std::vector<uint8_t> encryptedBytes = DecryptBytes(keyBytes, textBytes);

    std::string result(encryptedBytes.size(), 0);
    for (int i = 0; i < result.size(); ++i) {
        result[i] = (char) encryptedBytes[i];
    }

    result.resize(cutoff);

    return result;
}

void RijndaelAlgorithm::InversedMixColumns(std::vector<std::vector<uint8_t>> &state) {
    uint8_t x1, x2, x3, x4;

    for (int i = 0; i < keySize; ++i) {
        x1 = state[0][i];
        x2 = state[1][i];
        x3 = state[2][i];
        x4 = state[3][i];

        state[0][i] = Multiplication(x1, 0x0e) ^ Multiplication(x2, 0x0b) ^ Multiplication(x3, 0x0d) ^ Multiplication(x4, 0x09);
        state[1][i] = Multiplication(x1, 0x09) ^ Multiplication(x2, 0x0e) ^ Multiplication(x3, 0x0b) ^ Multiplication(x4, 0x0d);
        state[2][i] = Multiplication(x1, 0x0d) ^ Multiplication(x2, 0x09) ^ Multiplication(x3, 0x0e) ^ Multiplication(x4, 0x0b);
        state[3][i] = Multiplication(x1, 0x0b) ^ Multiplication(x2, 0x0d) ^ Multiplication(x3, 0x09) ^ Multiplication(x4, 0x0e);
    }
}

uint8_t xtime(uint8_t x) {
    return (x << 1u) ^ (((x >> 7u) & 1u) * 0x1b);
}

void RijndaelAlgorithm::MixColumns(std::vector<std::vector<uint8_t>> &state) {
    uint8_t x1, x2, x3;

    for (int i = 0; i < keySize; ++i) {
        x1 = state[0][i];
        x3 = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];

        x2 = state[0][i] ^ state[1][i];
        x2 = xtime(x2);
        state[0][i] ^= x2 ^ x3;

        x2 = state[1][i] ^ state[2][i];
        x2 = xtime(x2);
        state[1][i] ^= x2 ^ x3;

        x2 = state[2][i] ^ state[3][i];
        x2 = xtime(x2);
        state[2][i] ^= x2 ^ x3;

        x2 = state[3][i] ^ x1;
        x2 = xtime(x2);
        state[3][i] ^= x2 ^ x3;
    }
}