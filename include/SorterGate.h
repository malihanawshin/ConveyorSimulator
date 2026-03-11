#pragma once
#include <string>

class SorterGate {
public:
    SorterGate();

    void setRouteLeft();
    void setRouteRight();

    std::string getPosition() const;

private:
    std::string position;
};
