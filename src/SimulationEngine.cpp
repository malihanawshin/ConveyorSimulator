#include "SimulationEngine.h"

#include <chrono>
#include <iostream>
#include <thread>

SimulationEngine::SimulationEngine() = default;

void SimulationEngine::run() {
    std::cout << "=== Conveyor Sorting Cell Simulation ===\n\n";

    printStatus();
    startupSequence();
    normalItemCycle();
    jamFaultCycle();
    resetSystem();

    std::cout << "\n=== Simulation finished ===\n";
}

void SimulationEngine::startupSequence() {
    std::cout << "\n--- Startup sequence ---\n";

    controller.start();
    std::cout << "Start command issued. State: " << controller.getStateName() << "\n";

    for (int i = 0; i < 3; ++i) {
        controller.tick();
        std::cout << "Tick " << i + 1 << " -> State: " << controller.getStateName() << "\n";
        sleepMs(400);
    }

    if (controller.getState() == MachineState::Running) {
        conveyor.start();
    }

    std::cout << "Conveyor running: " << (conveyor.isRunning() ? "YES" : "NO") << "\n";
}

void SimulationEngine::normalItemCycle() {
    std::cout << "\n--- Normal item cycle ---\n";

    entrySensor.detectItem();
    std::cout << "Item detected at entry sensor\n";

    for (int i = 0; i < 2; ++i) {
        entrySensor.tick();
        std::cout << "Sensor blocked ticks: " << entrySensor.getBlockedTicks() << "\n";
        sleepMs(300);
    }

    gate.setRouteLeft();
    std::cout << "Gate switched to: " << gate.getPosition() << "\n";
    std::cout << "Item sorted to LEFT\n";

    entrySensor.clear();
    std::cout << "Sensor state: " << (entrySensor.isBlocked() ? "BLOCKED" : "CLEAR") << "\n";
}

void SimulationEngine::jamFaultCycle() {
    std::cout << "\n--- Jam fault cycle ---\n";

    entrySensor.detectItem();
    std::cout << "Second item detected at entry sensor\n";

    for (int i = 0; i < 5; ++i) {
        entrySensor.tick();
        std::cout << "Sensor blocked ticks: " << entrySensor.getBlockedTicks() << "\n";
        sleepMs(300);
    }

    if (entrySensor.getBlockedTicks() > 3) {
        alarms.raiseAlarm("Jam detected: sensor blocked too long");
        controller.triggerFault(alarms.getLatestAlarm());
        conveyor.stop();
    }

    printStatus();
}

void SimulationEngine::resetSystem() {
    std::cout << "\n--- Reset system ---\n";

    controller.resetFault();
    alarms.clearAlarms();
    entrySensor.clear();

    printStatus();
}

void SimulationEngine::printStatus() const {
    std::cout << "Machine state: " << controller.getStateName() << "\n";
    std::cout << "Conveyor running: " << (conveyor.isRunning() ? "YES" : "NO") << "\n";
    std::cout << "Gate position: " << gate.getPosition() << "\n";
    std::cout << "Sensor state: " << (entrySensor.isBlocked() ? "BLOCKED" : "CLEAR") << "\n";
    std::cout << "Latest alarm: " << alarms.getLatestAlarm() << "\n";
}

void SimulationEngine::sleepMs(int milliseconds) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
