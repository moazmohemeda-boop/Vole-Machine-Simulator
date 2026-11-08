#include "ALU.h"

std::uint8_t ALU::addInteger(std::uint8_t a, std::uint8_t b) {
    return a + b;
}

std::uint8_t ALU::bitwiseAnd(std::uint8_t a, std::uint8_t b) {
    return a & b;
}

std::uint8_t ALU::bitwiseOr(std::uint8_t a, std::uint8_t b) {
    return a | b;
}

std::uint8_t ALU::bitwiseXor(std::uint8_t a, std::uint8_t b) {
    return a ^ b;
}

std::uint8_t ALU::rotateRight(std::uint8_t value, std::uint8_t count) {
    count = count % 8;
    if (count == 0) return value;
    return (value >> count) | (value << (8 - count));
}

std::uint8_t ALU::addFloat(std::uint8_t a, std::uint8_t b) {
    auto voleToFloat = [](std::uint8_t val) -> float {
        if (val == 0) return 0.0f;
        int sign = (val & 0x80) ? -1 : 1;
        int exp = ((val & 0x70) >> 4) - 4;
        float mantissa = (val & 0x0F) / 16.0f;
        return sign * mantissa * (1 << (exp + 4)) / 16.0f; 
    };

    float fA = voleToFloat(a);
    float fB = voleToFloat(b);
    float sum = fA + fB;

    if (sum == 0.0f) return 0x00;

    std::uint8_t signBit = (sum < 0) ? 0x80 : 0x00;
    if (sum < 0) sum = -sum;

    int exp = 0;
    std::uint8_t mantissaBits = 0;

    while (sum >= 1.0f && exp < 3) { 
        sum /= 2.0f; 
        exp++; 
    }
    
    while (sum < 0.5f && exp > -4) { 
        sum *= 2.0f; 
        exp--; 
    }

    if (sum >= 1.0f) { 
        exp = 3; 
        mantissaBits = 0x0F; 
    } else {
        mantissaBits = static_cast<std::uint8_t>(sum * 16.0f) & 0x0F;
    }

    if (mantissaBits == 0) return 0x00;

    std::uint8_t expBits = ((exp + 4) & 0x07) << 4;

    return signBit | expBits | mantissaBits;
}
