#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "Memory.h"
#include "RegisterFile.h"
#include "ALU.h"
#include "Instruction.h"
#include "InstructionDecoder.h"

class CPU {
private:
    Memory& memory;                 // non-owning reference
    RegisterFile registers;
    ALU alu;
    InstructionDecoder decoder;
    std::uint16_t pc;               // uint16_t so 256 = end of memory
    Instruction ir;
    bool halted;

    std::uint16_t fetch();
    void execute(const Instruction& instruction);

public:
    explicit CPU(Memory& memory);

    // Runs one fetch-decode-execute cycle.
    // Returns true if the CPU can continue, false if halted / invalid / end of memory.
    bool step();
    void reset();

    std::uint16_t getPC() const;
    const Instruction& getIR() const;
    const RegisterFile& getRegisters() const;
    bool isHalted() const;
};

#endif
