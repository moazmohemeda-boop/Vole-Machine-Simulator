#include "SimulatorEngine.h"
#include <cstdint>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

SimulatorEngine::SimulatorEngine() : memory(), cpu(memory) {}

bool SimulatorEngine::loadProgramFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::vector<uint16_t> program;
    std::string hexStr;

    while (file >> hexStr) {
        if (hexStr.length() == 4) {
            uint16_t word = static_cast<uint16_t>(std::stoul(hexStr, nullptr, 16));
            program.push_back(word >> 8);
            program.push_back(word & 0xFF);
        }
    }

    reset();
    memory.loadProgram(program, 0);
    return true;
}

void SimulatorEngine::runAll() {
    while (!cpu.isHalted() && cpu.step());
}

bool SimulatorEngine::runStep() {
    return cpu.step();
}

void SimulatorEngine::reset() {
    memory.clear();
    cpu.reset();
}

Memory& SimulatorEngine::getMemory() { return memory; }
CPU& SimulatorEngine::getCPU() { return cpu; }
const Memory& SimulatorEngine::getMemory() const { return memory; }
const CPU& SimulatorEngine::getCPU() const { return cpu; }