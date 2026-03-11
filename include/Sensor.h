#pragma once

class Sensor {
public:
    Sensor();

    void detectItem();
    void clear();
    void tick();

    bool isBlocked() const;
    int getBlockedTicks() const;

private:
    bool blocked;
    int blockedTicks;
};
