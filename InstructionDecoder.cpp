#include "InstructionDecoder.h"

Instruction InstructionDecoder::decode(std::uint16_t rawWord) {
    std::uint8_t op   = (rawWord >> 12) & 0xF;  // 1st nibble
    std::uint8_t regR = (rawWord >> 8)  & 0xF;  // 2nd nibble
    std::uint8_t x    = (rawWord >> 4)  & 0xF;  // 3rd nibble
    std::uint8_t y    = rawWord         & 0xF;  // 4th nibble
    std::uint8_t xy   = rawWord         & 0xFF; // last byte (address / immediate)

    Opcode opcode = Opcode::INVALID;
    bool valid = true;

    switch (op) {
        case 0x1: opcode = Opcode::LOAD_MEM;  break;
        case 0x2: opcode = Opcode::LOAD_VAL;  break;
        case 0x3: opcode = Opcode::STORE;     break;
        case 0x4:
            // MOVE format is 40RS -> second nibble must be 0
            if (regR == 0) opcode = Opcode::MOVE;
            else           valid = false;
            break;
        case 0x5: opcode = Opcode::ADD_INT;   break;
        case 0x6: opcode = Opcode::ADD_FLOAT; break;
        case 0xB: opcode = Opcode::JUMP_EQ;   break;
        case 0xC:
            // HALT format is C000
            if ((rawWord & 0x0FFF) == 0) opcode = Opcode::HALT;
            else                         valid = false;
            break;
        default:  valid = false; break;
    }

    if (!valid) opcode = Opcode::INVALID;

    return Instruction(rawWord, opcode, regR, x, y, xy, xy, valid);
}
