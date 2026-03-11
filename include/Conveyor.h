#pragma once

class Conveyor {
public:
    Conveyor();

    void start();
    void stop();

    bool isRunning() const;

private:
    bool running;
};
