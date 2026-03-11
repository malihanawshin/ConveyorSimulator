#include <gtest/gtest.h>
#include "MachineController.h"

TEST(MachineControllerTest, StartsFromIdleToRunningAfterThreeTicks) {
    MachineController controller;

    EXPECT_EQ(controller.getStateName(), "Idle");

    controller.start();
    EXPECT_EQ(controller.getStateName(), "Starting");

    controller.tick();
    EXPECT_EQ(controller.getStateName(), "Starting");

    controller.tick();
    EXPECT_EQ(controller.getStateName(), "Starting");

    controller.tick();
    EXPECT_EQ(controller.getStateName(), "Running");
}

TEST(MachineControllerTest, FaultMovesControllerToFaultState) {
    MachineController controller;

    controller.triggerFault("Sensor timeout");

    EXPECT_EQ(controller.getStateName(), "Fault");
    EXPECT_EQ(controller.getLastAlarm(), "Sensor timeout");
}

TEST(MachineControllerTest, ResetAfterFaultReturnsToIdle) {
    MachineController controller;

    controller.triggerFault("Jam detected");
    controller.resetFault();

    EXPECT_EQ(controller.getStateName(), "Idle");
    EXPECT_EQ(controller.getLastAlarm(), "");
}

TEST(MachineControllerTest, EmergencyStopOverridesNormalOperation) {
    MachineController controller;

    controller.start();
    controller.tick();
    controller.tick();
    controller.tick();

    EXPECT_EQ(controller.getStateName(), "Running");

    controller.emergencyStop();

    EXPECT_EQ(controller.getStateName(), "EmergencyStop");
    EXPECT_EQ(controller.getLastAlarm(), "Emergency stop activated");
}
