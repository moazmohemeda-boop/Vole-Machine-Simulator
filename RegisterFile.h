#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include <array>
#include <cstdint>

class RegisterFile {
private:
    std::array<std::uint8_t, 16> registers;

public:
    RegisterFile();

    std::uint8_t read(std::uint8_t index) const;
    void write(std::uint8_t index, std::uint8_t value);
    void clear();
};

#endif