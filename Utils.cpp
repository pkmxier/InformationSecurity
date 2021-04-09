//
// Created by pkmixer on 07.04.2021.
//

#include "Utils.h"

uint32_t findNext2Power(uint32_t n) {
    --n;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

uint8_t Multiplication(uint8_t a, uint8_t b) {
    uint8_t result = 0;

    while (b) {
        if (b & 1u) {
            result ^= a;
        }

        if (a & 0x80) {
            a = (a << 1u) ^ 0b00011011;
        } else {
            a <<= 1u;
        }

        b >>= 1u;
    }
    return result;
}

uint8_t Exp(uint8_t value, int power) {
    if (power == 0) {
        return 1u;
    }

    uint8_t current = Exp(value, power / 2);
    uint8_t square = Multiplication(current, current);

    return power % 2 == 0 ? square : Multiplication(square, value);
}

uint8_t InverseOf(uint8_t n) {
    return Exp(n, 254);
}

uint8_t XOR(uint8_t byte) {
    byte = (byte >> 4u) ^ (byte & (0xff >> 4u));
    byte = (byte >> 2u) ^ (byte & (0xff >> 6u));
    return (byte >> 1u) ^ (byte & 1u);
}

std::uint8_t rotl(std::uint8_t v, std::uint8_t shift) {
    std::uint8_t s =  shift >= 0 ? shift % 8 : -((-shift) % 8);
    return (v << s) | (v >> (8 - s));
}