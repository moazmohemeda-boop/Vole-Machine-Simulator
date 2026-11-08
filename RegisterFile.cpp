#include "RegisterFile.h"
#include <stdexcept>

RegisterFile::RegisterFile() {
    clear();
}

std::uint8_t RegisterFile::read(std::uint8_t index) const {
    if (index >= 16) {
        throw std::out_of_range("Register index out of bounds (0-15 allowed).");
    }
    if (index == 0) {
        return 0;
    }
    return registers[index];
}

void RegisterFile::write(std::uint8_t index, std::uint8_t value) {
    if (index >= 16) {
        throw std::out_of_range("Register index out of bounds (0-15 allowed).");
    }
    if (index == 0) {
        return;
    }
    registers[index] = value;
}

void RegisterFile::clear() {
    registers.fill(0);
}