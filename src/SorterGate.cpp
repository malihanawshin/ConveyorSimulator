#include "SorterGate.h"

SorterGate::SorterGate() : position("CENTER") {}

void SorterGate::setRouteLeft() {
    position = "LEFT";
}

void SorterGate::setRouteRight() {
    position = "RIGHT";
}

std::string SorterGate::getPosition() const {
    return position;
}
