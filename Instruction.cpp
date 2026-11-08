#include "Instruction.h"

Instruction::Instruction() 
    : rawWord(0), opcode(Opcode::INVALID), regR(0), operandX(0), operandY(0), address(0), immediate(0), valid(false) {}

Instruction::Instruction(std::uint16_t word) : rawWord(word), valid(true) {
    std::uint8_t opPart = (word >> 12) & 0x0F;
    regR         = (word >> 8)  & 0x0F;
    operandX     = (word >> 4)  & 0x0F;
    operandY     = word         & 0x0F;
    address      = word & 0xFF;
    immediate    = word & 0xFF;

    switch (opPart) {
        case 0x1: opcode = Opcode::LOAD_MEM;  break;
        case 0x2: opcode = Opcode::LOAD_VAL;  break;
        case 0x3: opcode = Opcode::STORE;     break;
        case 0x4: opcode = Opcode::MOVE;      break;
        case 0x5: opcode = Opcode::ADD_INT;   break;
        case 0x6: opcode = Opcode::ADD_FLOAT; break;
        case 0x7: opcode = Opcode::OR;        break;
        case 0x8: opcode = Opcode::AND;       break;
        case 0x9: opcode = Opcode::XOR;       break;
        case 0xA: opcode = Opcode::ROTATE;    break;
        case 0xB: opcode = Opcode::JUMP_EQ;   break;
        case 0xC: opcode = Opcode::HALT;      break;
        default: 
            opcode = Opcode::INVALID;
            valid = false;
            break;
    }
}

std::uint16_t Instruction::getRawWord() const { return rawWord; }
Opcode Instruction::getOpcode() const { return opcode; }
std::uint8_t Instruction::getRegR() const { return regR; }
std::uint8_t Instruction::getOperandX() const { return operandX; }
std::uint8_t Instruction::getOperandY() const { return operandY; }
std::uint8_t Instruction::getAddress() const { return address; }
std::uint8_t Instruction::getImmediate() const { return immediate; }
bool Instruction::isValid() const { return valid; }
