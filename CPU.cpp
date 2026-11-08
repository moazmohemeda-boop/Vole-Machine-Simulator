#include "CPU.h"
#include <iostream>

CPU::CPU(Memory& mem)
    : memory(mem), registers(), alu(), decoder(), pc(0), ir(), halted(false) {}

std::uint16_t CPU::fetch() {
    // Need 2 bytes: pc and pc+1 must both be inside memory
    if (pc + 1 >= memory.size()) {
        halted = true;
        return 0;
    }
    std::uint16_t word = memory.readWord(pc);
    pc += 2;
    return word;
}

bool CPU::step() {
    if (halted) return false;

    std::uint16_t word = fetch();
    if (halted) return false;       // end of memory

    ir = decoder.decode(word);
    if (!ir.isValid()) {
        halted = true;
        return false;
    }

    execute(ir);
    return !halted;
}

void CPU::execute(const Instruction& inst) {
    std::uint8_t r = inst.getRegR();

    switch (inst.getOpcode()) {
        case Opcode::LOAD_MEM:   // 1RXY : R = memory[XY]
            registers.write(r, memory.readByte(inst.getAddress()));
            break;

        case Opcode::LOAD_VAL:   // 2RXY : R = XY
            registers.write(r, inst.getImmediate());
            break;

        case Opcode::STORE:      // 3RXY : memory[XY] = R  (address 00 = screen)
            if (inst.getAddress() == 0x00) {
                std::cout << "Screen: 0x" << std::hex << std::uppercase
                          << static_cast<int>(registers.read(r))
                          << std::dec << std::endl;
            }
            memory.writeByte(inst.getAddress(), registers.read(r));
            break;

        case Opcode::MOVE:       // 40RS : S = R
            registers.write(inst.getOperandY(), registers.read(inst.getOperandX()));
            break;

        case Opcode::ADD_INT:    // 5RST : R = S + T (two's complement)
            registers.write(r, alu.addInteger(registers.read(inst.getOperandX()),
                                              registers.read(inst.getOperandY())));
            break;

        case Opcode::ADD_FLOAT:  // 6RST : R = S + T (floating point)
            registers.write(r, alu.addFloat(registers.read(inst.getOperandX()),
                                            registers.read(inst.getOperandY())));
            break;

        case Opcode::JUMP_EQ:    // BRXY : if R == R0 then PC = XY
            if (registers.read(r) == registers.read(0)) {
                pc = inst.getAddress();
            }
            break;

        case Opcode::HALT:       // C000
            halted = true;
            break;

        default:
            halted = true;
            break;
    }
}

void CPU::reset() {
    registers.clear();
    pc = 0;
    ir = Instruction();
    halted = false;
}

std::uint16_t CPU::getPC() const { return pc; }
const Instruction& CPU::getIR() const { return ir; }
const RegisterFile& CPU::getRegisters() const { return registers; }
bool CPU::isHalted() const { return halted; }
