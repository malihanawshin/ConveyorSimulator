#include "SimulationEngine.h"

#include <chrono>
#include <iostream>
#include <thread>

SimulationEngine::SimulationEngine() = default;

void SimulationEngine::run() {
    std::cout << "=== Conveyor Sorting Cell Simulation ===\n\n";

    printStatus();

    std::cout << "\n--- Startup step ---\n";
    startupStep();
    printStatus();
    sleepMs(400);

    std::cout << "\n--- Normal item step ---\n";
    //processNormalItemStep();
    processNormalItemStep(ItemType::TypeA);
    printStatus();
    sleepMs(400);

    std::cout << "\n--- Jam fault step ---\n";
    processJamFaultStep();
    printStatus();
    sleepMs(400);

    std::cout << "\n--- Reset step ---\n";
    resetStep();
    printStatus();

    std::cout << "\n=== Simulation finished ===\n";
}

void SimulationEngine::startupStep() {
    controller.start();

    for (int i = 0; i < 3; ++i) {
        controller.tick();
    }

    if (controller.getState() == MachineState::Running) {
        conveyor.start();
    }
}

void SimulationEngine::processNormalItemStep(ItemType itemType) {
    if (controller.getState() != MachineState::Running) {
        return;
    }

    entrySensor.detectItem();

    for (int i = 0; i < 2; ++i) {
        entrySensor.tick();
    }

    switch (itemType) {
        case ItemType::TypeA:
            gate.setRouteLeft();
            break;
        case ItemType::TypeB:
            gate.setRouteRight();
            break;
    }

    entrySensor.clear();
}


void SimulationEngine::processJamFaultStep() {
    if (controller.getState() != MachineState::Running) {
        return;
    }

    entrySensor.detectItem();

    for (int i = 0; i < 5; ++i) {
        entrySensor.tick();
    }

    if (entrySensor.getBlockedTicks() > 3) {
        alarms.raiseAlarm("Jam detected: sensor blocked too long");
        controller.triggerFault(alarms.getLatestAlarm());
        conveyor.stop();
    }
}

void SimulationEngine::resetStep() {
    controller.resetFault();
    alarms.clearAlarms();
    entrySensor.clear();
}

SimulationSnapshot SimulationEngine::getSnapshot() const {
    return SimulationSnapshot{
        controller.getStateName(),
        conveyor.isRunning(),
        gate.getPosition(),
        entrySensor.isBlocked(),
        entrySensor.getBlockedTicks(),
        alarms.getLatestAlarm()
    };
}

std::vector<std::string> SimulationEngine::buildStatusLines() const {
    SimulationSnapshot snapshot = getSnapshot();

    return {
        "Machine state: " + snapshot.machineState,
        std::string("Conveyor running: ") + (snapshot.conveyorRunning ? "YES" : "NO"),
        "Gate position: " + snapshot.gatePosition,
        std::string("Sensor state: ") + (snapshot.sensorBlocked ? "BLOCKED" : "CLEAR"),
        "Sensor blocked ticks: " + std::to_string(snapshot.sensorBlockedTicks),
        "Latest alarm: " + snapshot.latestAlarm
    };
}

void SimulationEngine::printStatus() const {
    for (const auto& line : buildStatusLines()) {
        std::cout << line << "\n";
    }
}

void SimulationEngine::sleepMs(int milliseconds) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
