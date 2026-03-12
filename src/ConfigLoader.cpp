#include "ConfigLoader.h"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

AppConfig ConfigLoader::loadFromFile(const std::string& filePath) {
    std::ifstream input(filePath);
    if (!input.is_open()) {
        throw std::runtime_error("Could not open config file: " + filePath);
    }

    json j;
    input >> j;

    AppConfig config;

    config.jamThresholdTicks = j.value("jam_threshold_ticks", 3);

    const auto& routing = j.at("routing");

    std::string typeARoute = routing.value("TypeA", "LEFT");
    std::string typeBRoute = routing.value("TypeB", "RIGHT");

    config.routingRules[ItemType::TypeA] = typeARoute;
    config.routingRules[ItemType::TypeB] = typeBRoute;

    return config;
}
