#include "MachineController.h"
#include <iostream>


int main() {
    MachineController controller;

    std::cout << "Initial state: " << controller.getStateName() << "\n";

    controller.start();
    std::cout << "After start command: " << controller.getStateName() << "\n";

    for (int i = 0; i < 4; i++) {
        controller.tick();
        std::cout << "Tick " << i + 1 << ": " << controller.getStateName() << "\n";
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    controller.triggerFault("Sensor timeout detected");
    std::cout << "Fault state: " << controller.getStateName()
              << " | Alarm: " << controller.getLastAlarm() << "\n";

    controller.resetFault();
    std::cout << "After reset: " << controller.getStateName() << "\n";

    controller.emergencyStop();
    std::cout << "Emergency state: " << controller.getStateName()
              << " | Alarm: " << controller.getLastAlarm() << "\n";

    return 0;
}
