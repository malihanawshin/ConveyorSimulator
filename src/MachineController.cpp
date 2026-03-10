#include "MachineController.h"

MachineController::MachineController()
    : currentState(MachineState::Idle), lastAlarm(""), startupTicks(0) {}

void MachineController::start() {
    if (currentState == MachineState::Idle || currentState == MachineState::Stopped) {
        currentState = MachineState::Starting;
        startupTicks = 0;
    }
}

void MachineController::stop() {
    if (currentState == MachineState::Running) {
        currentState = MachineState::Stopped;
    }
}

void MachineController::emergencyStop() {
    currentState = MachineState::EmergencyStop;
    lastAlarm = "Emergency stop activated";
}

void MachineController::triggerFault(const std::string& message) {
    currentState = MachineState::Fault;
    lastAlarm = message;
}

void MachineController::resetFault() {
    if (currentState == MachineState::Fault || currentState == MachineState::EmergencyStop) {
        currentState = MachineState::Idle;
        lastAlarm.clear();
        startupTicks = 0;
    }
}

void MachineController::tick() {
    if (currentState == MachineState::Starting) {
        startupTicks++;
        if (startupTicks >= 3) {
            currentState = MachineState::Running;
        }
    }
}

MachineState MachineController::getState() const {
    return currentState;
}

std::string MachineController::getStateName() const {
    switch (currentState) {
        case MachineState::Idle: return "Idle";
        case MachineState::Starting: return "Starting";
        case MachineState::Running: return "Running";
        case MachineState::Stopped: return "Stopped";
        case MachineState::Fault: return "Fault";
        case MachineState::EmergencyStop: return "EmergencyStop";
        default: return "Unknown";
    }
}

std::string MachineController::getLastAlarm() const {
    return lastAlarm;
}
