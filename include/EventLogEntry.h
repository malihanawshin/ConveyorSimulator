#pragma once

#include <string>

struct EventLogEntry {
    std::string timestamp;
    std::string type;
    std::string message;
};
