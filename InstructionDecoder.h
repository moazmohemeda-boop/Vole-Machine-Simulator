#ifndef INSTRUCTIONDECODER_H
#define INSTRUCTIONDECODER_H

#include <cstdint>
#include "Instruction.h"

class InstructionDecoder {
public:
    // Decodes a 16-bit word (OXYZ nibbles) into an Instruction object.
    Instruction decode(std::uint16_t rawWord);
};

#endif
