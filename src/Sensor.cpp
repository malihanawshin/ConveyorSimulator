#include "Sensor.h"

Sensor::Sensor() : blocked(false), blockedTicks(0) {}

void Sensor::detectItem() {
    blocked = true;
    blockedTicks = 0;
}

void Sensor::clear() {
    blocked = false;
    blockedTicks = 0;
}

void Sensor::tick() {
    if (blocked) {
        blockedTicks++;
    }
}

bool Sensor::isBlocked() const {
    return blocked;
}

int Sensor::getBlockedTicks() const {
    return blockedTicks;
}
