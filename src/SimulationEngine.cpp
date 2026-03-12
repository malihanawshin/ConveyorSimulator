#include <QDateTime>
#include "SimulationEngine.h"

#include <chrono>
#include <iostream>
#include <thread>

SimulationEngine::SimulationEngine(const std::string& configPath)
    : config(ConfigLoader::loadFromFile(configPath)) {}

void SimulationEngine::run() {
    std::cout << "=== Conveyor Sorting Cell Simulation ===\n\n";

    printStatus();

    std::cout << "\n--- Startup step ---\n";
    startupStep();
    printStatus();
    sleepMs(400);

    std::cout << "\n--- Normal item step (TypeA) ---\n";
    processNormalItemStep(ItemType::TypeA);
    printStatus();
    sleepMs(400);

    std::cout << "\n--- Normal item step (TypeB) ---\n";
    processNormalItemStep(ItemType::TypeB);
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

void SimulationEngine::addEvent(const std::string& type, const std::string& message) {
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    eventHistory.push_back({timestamp.toStdString(), type, message});
}

void SimulationEngine::startupStep() {
    controller.start();
    addEvent("EVENT", "Operator pressed Start Machine");

    for (int i = 0; i < 3; ++i) {
        controller.tick();
    }

    if (controller.getState() == MachineState::Running) {
        conveyor.start();
        addEvent("EVENT", "Machine entered RUNNING state");
        addEvent("EVENT", "Conveyor started");
    }
}

void SimulationEngine::processNormalItemStep(ItemType itemType) {
    if (controller.getState() != MachineState::Running) {
        addEvent("EVENT", "Ignored item process command because machine is not RUNNING");
        return;
    }

    entrySensor.detectItem();
    addEvent("EVENT", "Entry sensor detected item");

    for (int i = 0; i < 2; ++i) {
        entrySensor.tick();
    }

    applyRouting(itemType);

    if (itemType == ItemType::TypeA) {
        addEvent("EVENT", "Processed TypeA item");
    } else {
        addEvent("EVENT", "Processed TypeB item");
    }

    addEvent("EVENT", "Item sorted to " + gate.getPosition());

    entrySensor.clear();
    addEvent("EVENT", "Entry sensor cleared");
}


void SimulationEngine::processJamFaultStep() {
    if (controller.getState() != MachineState::Running) {
        addEvent("EVENT", "Ignored jam simulation because machine is not RUNNING");
        return;
    }

    entrySensor.detectItem();
    addEvent("EVENT", "Jam simulation started: entry sensor blocked");

    for (int i = 0; i < config.jamThresholdTicks + 2; ++i) {
        entrySensor.tick();
    }

    if (entrySensor.getBlockedTicks() > config.jamThresholdTicks) {
        alarms.raiseAlarm("Jam detected: sensor blocked too long");
        controller.triggerFault(alarms.getLatestAlarm());
        conveyor.stop();

        addEvent("ALARM", alarms.getLatestAlarm());
        addEvent("EVENT", "Conveyor stopped due to jam fault");
    }
}


void SimulationEngine::resetStep() {
    controller.resetFault();
    alarms.clearAlarms();
    entrySensor.clear();
    addEvent("EVENT", "Operator reset the system");
}


const std::vector<EventLogEntry>& SimulationEngine::getEventHistory() const {
    return eventHistory;
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

void SimulationEngine::applyRouting(ItemType itemType) {
    std::string route = config.routingRules[itemType];

    if (route == "LEFT") {
        gate.setRouteLeft();
    } else if (route == "RIGHT") {
        gate.setRouteRight();
    } else {
        throw std::runtime_error("Invalid route in config");
    }
}

void SimulationEngine::printStatus() const {
    for (const auto& line : buildStatusLines()) {
        std::cout << line << "\n";
    }
}

void SimulationEngine::sleepMs(int milliseconds) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
