//
// Created by pkmixer on 07.04.2021.
//

#ifndef AES_UTILS_H
#define AES_UTILS_H

#include <cstdint>

uint32_t findNext2Power(uint32_t n);
uint8_t Multiplication(uint8_t a, uint8_t b);
uint8_t Exp(uint8_t value, int power);
uint8_t InverseOf(uint8_t n);
uint8_t XOR(uint8_t byte);
std::uint8_t rotl(std::uint8_t v, std::uint8_t shift);

#endif //AES_UTILS_H
