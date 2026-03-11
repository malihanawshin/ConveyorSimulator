#include <gtest/gtest.h>
#include "Sensor.h"

TEST(SensorTest, DetectItemBlocksSensor) {
    Sensor sensor;

    EXPECT_FALSE(sensor.isBlocked());
    EXPECT_EQ(sensor.getBlockedTicks(), 0);

    sensor.detectItem();

    EXPECT_TRUE(sensor.isBlocked());
    EXPECT_EQ(sensor.getBlockedTicks(), 0);
}

TEST(SensorTest, TickIncreasesBlockedTicksWhenBlocked) {
    Sensor sensor;

    sensor.detectItem();
    sensor.tick();
    sensor.tick();

    EXPECT_TRUE(sensor.isBlocked());
    EXPECT_EQ(sensor.getBlockedTicks(), 2);
}

TEST(SensorTest, ClearResetsSensorState) {
    Sensor sensor;

    sensor.detectItem();
    sensor.tick();
    sensor.tick();
    sensor.clear();

    EXPECT_FALSE(sensor.isBlocked());
    EXPECT_EQ(sensor.getBlockedTicks(), 0);
}
