#pragma once
#include "MachineState.h"
#include <string>

class MachineController {
public:
    MachineController();

    void start();
    void stop();
    void emergencyStop();
    void triggerFault(const std::string& message);
    void resetFault();
    void tick();

    MachineState getState() const;
    std::string getStateName() const;
    std::string getLastAlarm() const;

private:
    MachineState currentState;
    std::string lastAlarm;
    int startupTicks;
};
