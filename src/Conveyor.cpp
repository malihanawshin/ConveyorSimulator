#include "Conveyor.h"

Conveyor::Conveyor() : running(false) {}

void Conveyor::start() {
    running = true;
}

void Conveyor::stop() {
    running = false;
}

bool Conveyor::isRunning() const {
    return running;
}
