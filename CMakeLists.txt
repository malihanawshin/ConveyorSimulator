cmake_minimum_required(VERSION 3.16)

project(ConveyorSimulator VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(conveyor_sim
    src/main.cpp
    src/MachineController.cpp
    src/Conveyor.cpp
    src/Sensor.cpp
    src/SorterGate.cpp
    src/AlarmManager.cpp
)

target_include_directories(conveyor_sim PRIVATE include)
