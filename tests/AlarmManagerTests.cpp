#include <gtest/gtest.h>
#include "AlarmManager.h"

TEST(AlarmManagerTest, NoActiveAlarmInitially) {
    AlarmManager alarms;

    EXPECT_FALSE(alarms.hasActiveAlarms());
    EXPECT_EQ(alarms.getLatestAlarm(), "No active alarms");
}

TEST(AlarmManagerTest, RaiseAlarmStoresLatestAlarm) {
    AlarmManager alarms;

    alarms.raiseAlarm("Motor overload");

    EXPECT_TRUE(alarms.hasActiveAlarms());
    EXPECT_EQ(alarms.getLatestAlarm(), "Motor overload");
    EXPECT_EQ(alarms.getAllAlarms().size(), 1);
}

TEST(AlarmManagerTest, ClearAlarmsRemovesAllActiveAlarms) {
    AlarmManager alarms;

    alarms.raiseAlarm("Alarm 1");
    alarms.raiseAlarm("Alarm 2");
    alarms.clearAlarms();

    EXPECT_FALSE(alarms.hasActiveAlarms());
    EXPECT_EQ(alarms.getLatestAlarm(), "No active alarms");
    EXPECT_EQ(alarms.getAllAlarms().size(), 0);
}
