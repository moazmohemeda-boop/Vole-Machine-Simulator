#include "InstructionDecoder.h"

Instruction InstructionDecoder::decode(uint16_t rawWord) {
    return Instruction(rawWord);
}