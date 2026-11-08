#include "Memory.h"
#include <stdexcept>

Memory::Memory() {
    clear();
}

std::uint8_t Memory::readByte(std::uint16_t address) const {
    if (address >= cells.size()) {
        throw std::out_of_range("Memory address out of bounds");
    }
    return cells[address];
}

void Memory::writeByte(std::uint16_t address, std::uint8_t value) {
    if (address >= cells.size()) {
        throw std::out_of_range("Memory address out of bounds");
    }
    cells[address] = value;
}

std::uint16_t Memory::readWord(std::uint16_t address) const {
    if (address + 1 >= cells.size()) {
        throw std::out_of_range("Memory word-read boundary violation");
    }
    std::uint16_t high = cells[address];
    std::uint16_t low = cells[address + 1];
    return (high << 8) | low;
}

void Memory::loadProgram(const std::vector<std::uint16_t>& program, std::uint16_t start) {
    if (start + program.size() > cells.size()) {
        throw std::out_of_range("Program exceeds memory size");
    }
    for (std::size_t i = 0; i < program.size(); ++i) {
        // لو حابب تخزن كل عنصر كـ 2 بايت أو بايت واحد حسب تصميمك
        cells[start + i] = static_cast<std::uint8_t>(program[i]);
    }
}

void Memory::clear() {
    cells.fill(0);
}

std::size_t Memory::size() const {
    return cells.size();
}