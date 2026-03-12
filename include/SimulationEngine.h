#pragma once

#include <string>
#include <vector>

#include "AlarmManager.h"
#include "ConfigLoader.h"
#include "Conveyor.h"
#include "ItemType.h"
#include "MachineController.h"
#include "Sensor.h"
#include "SorterGate.h"

struct SimulationSnapshot {
    std::string machineState;
    bool conveyorRunning;
    std::string gatePosition;
    bool sensorBlocked;
    int sensorBlockedTicks;
    std::string latestAlarm;
};

class SimulationEngine {
public:
    explicit SimulationEngine(const std::string& configPath = "config/routing.json");

    void run();

    void startupStep();
    void processNormalItemStep(ItemType itemType);
    void processJamFaultStep();
    void resetStep();

    SimulationSnapshot getSnapshot() const;

private:
    MachineController controller;
    Conveyor conveyor;
    Sensor entrySensor;
    SorterGate gate;
    AlarmManager alarms;
    AppConfig config;

    std::vector<std::string> buildStatusLines() const;
    void applyRouting(ItemType itemType);
    void printStatus() const;
    void sleepMs(int milliseconds) const;
};
