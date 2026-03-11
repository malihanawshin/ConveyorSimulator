#pragma once
#include <string>
#include <vector>

class AlarmManager {
public:
    void raiseAlarm(const std::string& message);
    void clearAlarms();

    bool hasActiveAlarms() const;
    std::string getLatestAlarm() const;
    const std::vector<std::string>& getAllAlarms() const;

private:
    std::vector<std::string> alarms;
};
