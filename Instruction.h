#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "Opcode.h"
#include <cstdint>

class Instruction {
private:
    std::uint16_t rawWord;
    Opcode opcode;
    std::uint8_t regR;
    std::uint8_t operandX;
    std::uint8_t operandY;
    std::uint8_t address;
    std::uint8_t immediate;
    bool valid;

public:
    Instruction();
    explicit Instruction(std::uint16_t word);

    std::uint16_t getRawWord() const;
    Opcode getOpcode() const;
    std::uint8_t getRegR() const;
    std::uint8_t getOperandX() const;
    std::uint8_t getOperandY() const;
    std::uint8_t getAddress() const;
    std::uint8_t getImmediate() const;
    bool isValid() const;
};

#endif // INSTRUCTION_H
