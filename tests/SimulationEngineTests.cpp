#include <gtest/gtest.h>
#include "SimulationEngine.h"

TEST(SimulationEngineTest, StartupStepMovesSystemToRunningAndStartsConveyor) {
    SimulationEngine engine;

    engine.startupStep();
    SimulationSnapshot snapshot = engine.getSnapshot();

    EXPECT_EQ(snapshot.machineState, "Running");
    EXPECT_TRUE(snapshot.conveyorRunning);
    EXPECT_EQ(snapshot.gatePosition, "CENTER");
    EXPECT_FALSE(snapshot.sensorBlocked);
    EXPECT_EQ(snapshot.latestAlarm, "No active alarms");
}

TEST(SimulationEngineTest, NormalItemStepRoutesItemWithoutAlarm) {
    SimulationEngine engine;

    engine.startupStep();
    engine.processNormalItemStep();

    SimulationSnapshot snapshot = engine.getSnapshot();

    EXPECT_EQ(snapshot.machineState, "Running");
    EXPECT_TRUE(snapshot.conveyorRunning);
    EXPECT_EQ(snapshot.gatePosition, "LEFT");
    EXPECT_FALSE(snapshot.sensorBlocked);
    EXPECT_EQ(snapshot.sensorBlockedTicks, 0);
    EXPECT_EQ(snapshot.latestAlarm, "No active alarms");
}

TEST(SimulationEngineTest, JamFaultStepTriggersFaultAndStopsConveyor) {
    SimulationEngine engine;

    engine.startupStep();
    engine.processJamFaultStep();

    SimulationSnapshot snapshot = engine.getSnapshot();

    EXPECT_EQ(snapshot.machineState, "Fault");
    EXPECT_FALSE(snapshot.conveyorRunning);
    EXPECT_TRUE(snapshot.sensorBlocked);
    EXPECT_GT(snapshot.sensorBlockedTicks, 3);
    EXPECT_EQ(snapshot.latestAlarm, "Jam detected: sensor blocked too long");
}

TEST(SimulationEngineTest, ResetStepReturnsSystemToIdleAndClearsAlarm) {
    SimulationEngine engine;

    engine.startupStep();
    engine.processJamFaultStep();
    engine.resetStep();

    SimulationSnapshot snapshot = engine.getSnapshot();

    EXPECT_EQ(snapshot.machineState, "Idle");
    EXPECT_FALSE(snapshot.sensorBlocked);
    EXPECT_EQ(snapshot.sensorBlockedTicks, 0);
    EXPECT_EQ(snapshot.latestAlarm, "No active alarms");
}
