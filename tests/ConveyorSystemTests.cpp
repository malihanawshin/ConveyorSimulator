#include <gtest/gtest.h>
#include "AlarmManager.h"
#include "Conveyor.h"
#include "MachineController.h"
#include "Sensor.h"
#include "SorterGate.h"

TEST(ConveyorSystemTest, NormalItemFlowStartsSystemAndSortsLeft) {
    MachineController controller;
    Conveyor conveyor;
    Sensor sensor;
    SorterGate gate;
    AlarmManager alarms;

    controller.start();
    controller.tick();
    controller.tick();
    controller.tick();

    ASSERT_EQ(controller.getStateName(), "Running");

    conveyor.start();
    EXPECT_TRUE(conveyor.isRunning());

    sensor.detectItem();
    sensor.tick();
    sensor.tick();

    gate.setRouteLeft();

    EXPECT_TRUE(sensor.isBlocked());
    EXPECT_EQ(sensor.getBlockedTicks(), 2);
    EXPECT_EQ(gate.getPosition(), "LEFT");
    EXPECT_FALSE(alarms.hasActiveAlarms());

    sensor.clear();
    EXPECT_FALSE(sensor.isBlocked());
}

TEST(ConveyorSystemTest, JamConditionTriggersFaultAndStopsConveyor) {
    MachineController controller;
    Conveyor conveyor;
    Sensor sensor;
    AlarmManager alarms;

    controller.start();
    controller.tick();
    controller.tick();
    controller.tick();
    conveyor.start();

    ASSERT_EQ(controller.getStateName(), "Running");
    ASSERT_TRUE(conveyor.isRunning());

    sensor.detectItem();
    for (int i = 0; i < 5; ++i) {
        sensor.tick();
    }

    if (sensor.getBlockedTicks() > 3) {
        alarms.raiseAlarm("Jam detected: sensor blocked too long");
        controller.triggerFault(alarms.getLatestAlarm());
        conveyor.stop();
    }

    EXPECT_EQ(controller.getStateName(), "Fault");
    EXPECT_EQ(controller.getLastAlarm(), "Jam detected: sensor blocked too long");
    EXPECT_TRUE(alarms.hasActiveAlarms());
    EXPECT_FALSE(conveyor.isRunning());
}
