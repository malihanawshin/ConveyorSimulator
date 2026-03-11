#include <chrono>
#include <iostream>
#include <thread>

#include "AlarmManager.h"
#include "Conveyor.h"
#include "MachineController.h"
#include "Sensor.h"
#include "SorterGate.h"

int main() {
    MachineController controller;
    Conveyor conveyor;
    Sensor entrySensor;
    SorterGate gate;
    AlarmManager alarms;

    std::cout << "Initial state: " << controller.getStateName() << "\n";

    controller.start();
    std::cout << "Start command issued. State: " << controller.getStateName() << "\n";

    for (int i = 0; i < 3; ++i) {
        controller.tick();
        std::cout << "Tick " << i + 1 << " -> State: " << controller.getStateName() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }

    if (controller.getStateName() == "Running") {
        conveyor.start();
        std::cout << "Conveyor running: " << (conveyor.isRunning() ? "YES" : "NO") << "\n";
    }

    std::cout << "\n--- Item arrives at sensor ---\n";
    entrySensor.detectItem();

    for (int i = 0; i < 2; ++i) {
        entrySensor.tick();
        std::cout << "Sensor blocked ticks: " << entrySensor.getBlockedTicks() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    gate.setRouteLeft();
    std::cout << "Gate switched to: " << gate.getPosition() << "\n";
    std::cout << "Item sorted to LEFT\n";

    entrySensor.clear();
    std::cout << "Sensor cleared: " << (entrySensor.isBlocked() ? "BLOCKED" : "CLEAR") << "\n";

    std::cout << "\n--- Simulating jam fault ---\n";
    entrySensor.detectItem();

    for (int i = 0; i < 5; ++i) {
        entrySensor.tick();
        std::cout << "Sensor blocked ticks: " << entrySensor.getBlockedTicks() << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    if (entrySensor.getBlockedTicks() > 3) {
        alarms.raiseAlarm("Jam detected: sensor blocked too long");
        controller.triggerFault(alarms.getLatestAlarm());
        conveyor.stop();
    }

    std::cout << "State after jam: " << controller.getStateName() << "\n";
    std::cout << "Latest alarm: " << alarms.getLatestAlarm() << "\n";
    std::cout << "Conveyor running: " << (conveyor.isRunning() ? "YES" : "NO") << "\n";

    std::cout << "\n--- Resetting system ---\n";
    controller.resetFault();
    alarms.clearAlarms();
    entrySensor.clear();

    std::cout << "State after reset: " << controller.getStateName() << "\n";
    std::cout << "Alarms active: " << (alarms.hasActiveAlarms() ? "YES" : "NO") << "\n";

    return 0;
}
