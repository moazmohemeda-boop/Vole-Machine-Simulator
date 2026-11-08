#include "CPU.h"
#include <iostream>

CPU::CPU(Memory& mem)
    : memory(mem), registers(), alu(), decoder(), pc(0), ir(), halted(false) {}

std::uint16_t CPU::fetch() {
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
    if (halted) return false;

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
        case Opcode::LOAD_MEM:
            registers.write(r, memory.readByte(inst.getAddress()));
            break;

        case Opcode::LOAD_VAL:
            registers.write(r, inst.getImmediate());
            break;

        case Opcode::STORE:
            if (inst.getAddress() == 0x00) {
                std::cout << "Screen: 0x" << std::hex << std::uppercase
                          << static_cast<int>(registers.read(r))
                          << std::dec << std::endl;
            }
            memory.writeByte(inst.getAddress(), registers.read(r));
            break;

        case Opcode::MOVE:
            registers.write(inst.getOperandY(), registers.read(inst.getOperandX()));
            break;

        case Opcode::ADD_INT:
            registers.write(r, alu.addInteger(registers.read(inst.getOperandX()),
                                              registers.read(inst.getOperandY())));
            break;

        case Opcode::ADD_FLOAT:
            registers.write(r, alu.addFloat(registers.read(inst.getOperandX()),
                                            registers.read(inst.getOperandY())));
            break;

        case Opcode::OR:
            registers.write(r, registers.read(inst.getOperandX()) | registers.read(inst.getOperandY()));
            break;

        case Opcode::AND:
            registers.write(r, registers.read(inst.getOperandX()) & registers.read(inst.getOperandY()));
            break;

        case Opcode::XOR:
            registers.write(r, registers.read(inst.getOperandX()) ^ registers.read(inst.getOperandY()));
            break;

        case Opcode::JUMP_EQ:
            if (registers.read(r) == registers.read(0)) {
                pc = inst.getAddress();
            }
            break;

        case Opcode::HALT:
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