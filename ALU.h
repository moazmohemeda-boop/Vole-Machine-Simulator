#ifndef ALU_H
#define ALU_H

#include <cstdint>

class ALU {
public:
    std::uint8_t addInteger(std::uint8_t a, std::uint8_t b);
    std::uint8_t addFloat(std::uint8_t a, std::uint8_t b);
    std::uint8_t bitwiseAnd(std::uint8_t a, std::uint8_t b);
    std::uint8_t bitwiseOr(std::uint8_t a, std::uint8_t b);
    std::uint8_t bitwiseXor(std::uint8_t a, std::uint8_t b);
    std::uint8_t rotateRight(std::uint8_t value, std::uint8_t count);
};

#endif // ALU_H
