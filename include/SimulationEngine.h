#pragma once

#include "AlarmManager.h"
#include "Conveyor.h"
#include "MachineController.h"
#include "Sensor.h"
#include "SorterGate.h"

class SimulationEngine {
public:
    SimulationEngine();
    void run();

private:
    MachineController controller;
    Conveyor conveyor;
    Sensor entrySensor;
    SorterGate gate;
    AlarmManager alarms;

    void startupSequence();
    void normalItemCycle();
    void jamFaultCycle();
    void resetSystem();
    void printStatus() const;
    void sleepMs(int milliseconds) const;
};
