#include "AlarmManager.h"

void AlarmManager::raiseAlarm(const std::string& message) {
    alarms.push_back(message);
}

void AlarmManager::clearAlarms() {
    alarms.clear();
}

bool AlarmManager::hasActiveAlarms() const {
    return !alarms.empty();
}

std::string AlarmManager::getLatestAlarm() const {
    if (alarms.empty()) {
        return "No active alarms";
    }
    return alarms.back();
}

const std::vector<std::string>& AlarmManager::getAllAlarms() const {
    return alarms;
}
