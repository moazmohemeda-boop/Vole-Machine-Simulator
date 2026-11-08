#ifndef SIMULATOR_ENGINE_H
#define SIMULATOR_ENGINE_H

#include <string>
#include <vector>
#include "Memory.h"
#include "CPU.h"

class SimulatorEngine {
private:
    Memory memory;
    CPU cpu;

public:
    SimulatorEngine();
    bool loadProgramFromFile(const std::string& filename);
    void runAll();
    bool runStep();
    void reset();

    Memory& getMemory();
    CPU& getCPU();
    const Memory& getMemory() const;
    const CPU& getCPU() const;
};

#endif