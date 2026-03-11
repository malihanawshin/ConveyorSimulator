#include <gtest/gtest.h>
#include "SimulationEngine.h"
#include "ItemType.h"

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

TEST(SimulationEngineTest, NormalItemStepRoutesTypeAToLeft) {
    SimulationEngine engine;

    engine.startupStep();
    engine.processNormalItemStep(ItemType::TypeA);

    SimulationSnapshot snapshot = engine.getSnapshot();

    EXPECT_EQ(snapshot.machineState, "Running");
    EXPECT_TRUE(snapshot.conveyorRunning);
    EXPECT_EQ(snapshot.gatePosition, "LEFT");
    EXPECT_FALSE(snapshot.sensorBlocked);
    EXPECT_EQ(snapshot.sensorBlockedTicks, 0);
    EXPECT_EQ(snapshot.latestAlarm, "No active alarms");
}

TEST(SimulationEngineTest, NormalItemStepRoutesTypeBToRight) {
    SimulationEngine engine;

    engine.startupStep();
    engine.processNormalItemStep(ItemType::TypeB);

    SimulationSnapshot snapshot = engine.getSnapshot();

    EXPECT_EQ(snapshot.machineState, "Running");
    EXPECT_TRUE(snapshot.conveyorRunning);
    EXPECT_EQ(snapshot.gatePosition, "RIGHT");
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
