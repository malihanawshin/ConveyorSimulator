#pragma once

#include <string>
#include <vector>

#include "AlarmManager.h"
#include "Conveyor.h"
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
    SimulationEngine();

    void run();

    void startupStep();
    void processNormalItemStep();
    void processJamFaultStep();
    void resetStep();

    SimulationSnapshot getSnapshot() const;

private:
    MachineController controller;
    Conveyor conveyor;
    Sensor entrySensor;
    SorterGate gate;
    AlarmManager alarms;

    std::vector<std::string> buildStatusLines() const;
    void printStatus() const;
    void sleepMs(int milliseconds) const;
};
