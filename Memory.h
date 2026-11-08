#ifndef MEMORY_H
#define MEMORY_H
#include <array>
#include <cstdint>
#include <vector>
#include <cstddef>

class Memory {
private:
    std::array<std::uint8_t, 256> cells;

public:
    Memory();

    std::uint8_t readByte(std::uint16_t address) const;
    void writeByte(std::uint16_t address, std::uint8_t value);
    std::uint16_t readWord(std::uint16_t address) const;
    void loadProgram(const std::vector<std::uint16_t>& program, std::uint16_t start);
    void clear();
    std::size_t size() const;
};

#endif