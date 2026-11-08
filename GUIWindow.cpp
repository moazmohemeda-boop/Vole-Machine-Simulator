#include "GUIWindow.h"
#include "SimulatorEngine.h"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

GUIWindow::GUIWindow() : engine(new SimulatorEngine()) {}

GUIWindow::~GUIWindow() {
    delete engine;
    engine = nullptr;
}

void GUIWindow::printMenu() const {
    std::cout << "\n=== Vole Machine Simulator ===\n"
              << "1. Load program from file\n"
              << "2. Run one step\n"
              << "3. Run all\n"
              << "4. Show CPU status\n"
              << "5. Show registers\n"
              << "6. Show memory\n"
              << "7. Reset\n"
              << "0. Exit\n"
              << "Choice: ";
}

void GUIWindow::loadProgram() {
    std::string path;
    std::cout << "File path: ";
    std::getline(std::cin, path);

    if (engine->loadProgramFromFile(path))
        std::cout << "Program loaded.\n";
    else
        std::cout << "Failed to load program.\n";
}

void GUIWindow::showStatus() const {
    std::cout << std::hex << std::uppercase << std::setfill('0')
              << "PC     : " << std::setw(2) << static_cast<int>(engine->getCPU().getPC()) << "\n"
              << "IR     : " << std::setw(4) << static_cast<int>(engine->getCPU().getIR().getRawWord()) << "\n"
              << std::dec << std::setfill(' ')
              << "Halted : " << (engine->getCPU().isHalted() ? "yes" : "no") << "\n";
}

void GUIWindow::showRegisters() const {
    std::cout << std::hex << std::uppercase << std::setfill('0');
    for (int i = 0; i < 16; ++i) {
        std::cout << "R" << i << " = " << std::setw(2)
                  << static_cast<int>(engine->getCPU().getRegisters().read(i)) << "\n";
    }
    std::cout << std::dec << std::setfill(' ');
}

void GUIWindow::showMemory() const {
    std::cout << std::hex << std::uppercase << std::setfill('0');
    for (int row = 0; row < 16; ++row) {
        std::cout << std::setw(2) << (row * 16) << ": ";
        for (int col = 0; col < 16; ++col) {
            uint16_t addr = static_cast<uint16_t>(row * 16 + col);
            std::cout << std::setw(2)
                      << static_cast<int>(engine->getMemory().readByte(addr)) << ' ';
        }
        std::cout << "\n";
    }
    std::cout << std::dec << std::setfill(' ');
}

void GUIWindow::runMenu() {
    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input.\n";
            choice = -1;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: loadProgram(); break;
            case 2: engine->runStep(); showStatus(); break;
            case 3: engine->runAll(); showStatus(); break;
            case 4: showStatus(); break;
            case 5: showRegisters(); break;
            case 6: showMemory(); break;
            case 7: engine->reset(); std::cout << "Machine reset.\n"; break;
            case 0: std::cout << "Goodbye.\n"; break;
            default: std::cout << "Unknown option.\n"; break;
        }
    }
}