#pragma once

#include <string>
#include <unordered_map>

#include "ItemType.h"

struct AppConfig {
    std::unordered_map<ItemType, std::string> routingRules;
    int jamThresholdTicks = 3;
};

class ConfigLoader {
public:
    static AppConfig loadFromFile(const std::string& filePath);
};
