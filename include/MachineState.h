#pragma once

enum class MachineState {
    Idle,
    Starting,
    Running,
    Fault,
    EmergencyStop,
    Stopped
};
