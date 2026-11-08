#ifndef GUIWINDOW_H
#define GUIWINDOW_H

#include <cstdint>
#include <string>

class SimulatorEngine;

class GUIWindow {
public:
    GUIWindow();
    ~GUIWindow();
    GUIWindow(const GUIWindow&) = delete;
    GUIWindow& operator=(const GUIWindow&) = delete;

    void runMenu();

private:
    SimulatorEngine* engine;

    void printMenu() const;
    void loadProgram();
    void showStatus() const;
    void showRegisters() const;
    void showMemory() const;
};

#endif